/*
  Project 3: Ultrasonic Parking Assistant
  Board: Arduino Uno R3
  NOTE: OLED is the 7-pin SPI SSD1306 module in the kit (not I2C).
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define TRIG_PIN   A2
#define ECHO_PIN   A3
#define BUZZER_PIN 6
#define GREEN_LED  2
#define YELLOW_LED 3
#define RED_LED    4

// SPI OLED (hardware SPI on Uno: SCK=13, MOSI=11)
#define OLED_CS    7
#define OLED_DC    8
#define OLED_RESET 12

Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

unsigned long lastBeep = 0;
unsigned long lastPrint = 0;
unsigned long beepInterval = 0;

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 10000); // 10ms timeout
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED init failed");
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  float dist = measureDistance();
  String zone;

  if (dist > 60 || dist < 0) {
    zone = "SAFE";
    digitalWrite(GREEN_LED, HIGH); digitalWrite(YELLOW_LED, LOW); digitalWrite(RED_LED, LOW);
    beepInterval = 0;
    noTone(BUZZER_PIN);
  } else if (dist > 30) {
    zone = "CAUTION";
    digitalWrite(GREEN_LED, LOW); digitalWrite(YELLOW_LED, HIGH); digitalWrite(RED_LED, LOW);
    beepInterval = 800;
  } else if (dist > 15) {
    zone = "CLOSE";
    digitalWrite(GREEN_LED, LOW); digitalWrite(YELLOW_LED, HIGH); digitalWrite(RED_LED, HIGH);
    beepInterval = 300;
  } else {
    zone = "DANGER";
    digitalWrite(GREEN_LED, LOW); digitalWrite(YELLOW_LED, LOW); digitalWrite(RED_LED, HIGH);
    tone(BUZZER_PIN, 2000); // continuous alarm
  }

  if ((zone == "CAUTION" || zone == "CLOSE") && millis() - lastBeep > beepInterval) {
    lastBeep = millis();
    tone(BUZZER_PIN, 1000, 100);
  }

  // OLED: large distance text
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(dist >= 0 ? String((int)dist) : "--");
  display.println(" cm");
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(zone);
  display.display();

  if (millis() - lastPrint > 500) {
    lastPrint = millis();
    Serial.print("Distance: "); Serial.print(dist);
    Serial.print(" cm | Zone: "); Serial.println(zone);
  }
}
