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

// Fix: 5-sample average to reduce spurious readings
float averagedDistance() {
  float sum = 0;
  int valid = 0;
  for (int i = 0; i < 5; i++) {
    float d = measureDistance();
    if (d > 0) { sum += d; valid++; }
    delay(10);
  }
  return valid ? sum / valid : -1;
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
  float dist = averagedDistance();
  String zone;

  if (dist > 60 || dist < 0) {
    zone = "SAFE";
    digitalWrite(GREEN_LED, HIGH); digitalWrite(YELLOW_LED, LOW); digitalWrite(RED_LED, LOW);
    beepInterval = 0;
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
    beepInterval = 0; // continuous handled separately
  }

  // Fix: non-blocking buzzer with clean state transitions
  static String lastZone = "";
  if (zone != lastZone) {
    noTone(BUZZER_PIN); // stop any playing tone on zone transition
    lastZone = zone;
    if (zone == "CAUTION" || zone == "CLOSE") {
      lastBeep = 0; // force immediate beep on transition
    }
  }

  if (zone == "DANGER") {
    tone(BUZZER_PIN, 2000); // continuous tone, no duration = stays on
  } else if (beepInterval > 0 && millis() - lastBeep > beepInterval) {
    lastBeep = millis();
    tone(BUZZER_PIN, 1000, 100);
  } else if (zone == "SAFE") {
    noTone(BUZZER_PIN);
  }

  // Fix: OLED proximity bar graph added alongside distance text
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(dist >= 0 ? String((int)dist) : "--");
  display.println(" cm");
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(zone);
  int barLen = constrain(map((int)dist, 0, 100, 100, 0), 0, 100);
  display.drawRect(0, 40, 104, 10, SSD1306_WHITE);
  display.fillRect(2, 42, barLen, 6, SSD1306_WHITE);
  display.display();

  if (millis() - lastPrint > 500) {
    lastPrint = millis();
    Serial.print("Distance: "); Serial.print(dist);
    Serial.print(" cm | Zone: "); Serial.println(zone);
  }
}
