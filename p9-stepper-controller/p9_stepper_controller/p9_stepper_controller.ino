/*
  Project 9: Stepper Motor Precision Positioner
  Board: Arduino Uno R3
  NOTE: Stepper driver pins shifted to 7-10 to reserve D11/D13 for SPI OLED.
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Stepper.h>

const int STEPS_PER_REV = 2048; // half-step effective count for 28BYJ-48

// ULN2003 IN1-IN3-IN2-IN4 wiring order required by Stepper library
Stepper myStepper(STEPS_PER_REV, 8, 10, 9, 7); // IN1=8, IN2=9, IN3=10, IN4=7

#define POT_PIN   A0
#define CW_BTN    2
#define CCW_BTN   3
#define HOME_BTN  4

// SPI OLED (hardware SPI on Uno: SCK=13, MOSI=11 - kept free of stepper pins)
#define OLED_CS    5
#define OLED_DC    6
#define OLED_RESET A1
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

long currentSteps = 0;   // absolute position
long targetSteps = 0;
String currentDir = "--";

bool lastCW = HIGH, lastCCW = HIGH, lastHome = HIGH;

void setup() {
  Serial.begin(115200);
  myStepper.setSpeed(10); // RPM - do not exceed ~15 for 28BYJ-48

  pinMode(CW_BTN, INPUT_PULLUP);
  pinMode(CCW_BTN, INPUT_PULLUP);
  pinMode(HOME_BTN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED init failed");
  }
  display.clearDisplay();
  display.display();
}

float stepsToAngle(long steps) {
  long wrapped = steps % STEPS_PER_REV;
  if (wrapped < 0) wrapped += STEPS_PER_REV;
  return wrapped * 360.0 / STEPS_PER_REV;
}

void updateOLED(String dir) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Current: "); display.print(stepsToAngle(currentSteps), 1); display.println((char)247);
  display.setCursor(0, 12);
  display.print("Target:  "); display.print(stepsToAngle(targetSteps), 1); display.println((char)247);
  display.setCursor(0, 24);
  display.print("Dir: "); display.println(dir);
  display.setCursor(0, 36);
  display.print("Steps: "); display.println(currentSteps);
  display.display();
}

void loop() {
  // Potentiometer sets target angle 0-360
  int potRaw = analogRead(POT_PIN);
  float targetAngle = map(potRaw, 0, 1023, 0, 360);
  targetSteps = (long)(targetAngle / 360.0 * STEPS_PER_REV);

  static long lastTargetSteps = -1;
  bool targetChanged = (targetSteps != lastTargetSteps);
  if (targetChanged) {
    lastTargetSteps = targetSteps;
  }

  bool cw = digitalRead(CW_BTN);
  bool ccw = digitalRead(CCW_BTN);
  bool home = digitalRead(HOME_BTN);

  bool actionTaken = false;

  // Latching switch handling: trigger step on any state toggle
  if (cw != lastCW) {
    myStepper.step(512); // +45 deg
    currentSteps += 512;
    currentDir = "CW";
    actionTaken = true;
    Serial.print("CW step, position: "); Serial.println(currentSteps);
  }
  if (ccw != lastCCW) {
    myStepper.step(-512); // -45 deg
    currentSteps -= 512;
    currentDir = "CCW";
    actionTaken = true;
    Serial.print("CCW step, position: "); Serial.println(currentSteps);
  }
  if (home != lastHome) {
    currentSteps = 0; // logical home reset
    currentDir = "HOME SET";
    actionTaken = true;
    Serial.println("Home position set (logical, no movement)");
  }

  if (actionTaken || targetChanged) {
    updateOLED(currentDir);
  }

  lastCW = cw; lastCCW = ccw; lastHome = home;
}
