/*
  Project 1: Smart Room Climate Monitor
  Board: ESP32 Dev Board
  Key Components: ESP32, DHT22, OLED (SPI SSD1306), Buzzer, Red & Green LEDs
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>

// ---------- Pin Definitions ----------
#define DHT_PIN      4
#define RED_LED      25
#define GREEN_LED    26
#define BUZZER_PIN   27

// SPI OLED pins (hardware SPI: SCK=GPIO18, MOSI=GPIO23)
#define OLED_CS      5
#define OLED_DC      2
#define OLED_RESET   21

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);
DHTesp dht;

unsigned long lastRead = 0;
unsigned long lastLog = 0;
const unsigned long READ_INTERVAL = 2000;
const unsigned long LOG_INTERVAL  = 5000;

float maxTemp = -100, minTemp = 200;

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED init failed");
    while (true) delay(10);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.display();
}

String comfortIndex(float t, float h) {
  if (t > 38 || h > 80) return "DANGER";
  if (t > 32 || h > 65) return "HOT";
  if (t < 20) return "COOL";
  return "COMFORT";
}

void loop() {
  unsigned long now = millis();

  if (now - lastRead >= READ_INTERVAL) {
    lastRead = now;
    TempAndHumidity data = dht.getTempAndHumidity();

    if (dht.getStatus() != 0) {
      Serial.println("DHT read error");
      return;
    }

    if (data.temperature > maxTemp) maxTemp = data.temperature;
    if (data.temperature < minTemp) minTemp = data.temperature;

    String status = comfortIndex(data.temperature, data.humidity);
    bool alert = (data.temperature > 38 || data.humidity > 80);

    digitalWrite(RED_LED, alert ? HIGH : LOW);
    digitalWrite(GREEN_LED, alert ? LOW : HIGH);
    if (alert) {
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Temp: "); display.print(data.temperature, 1); display.println(" C");
    display.setCursor(0, 12);
    display.print("Hum:  "); display.print(data.humidity, 1); display.println(" %");
    display.setCursor(0, 24);
    display.print("Status: "); display.println(status);
    display.display();

    if (now - lastLog >= LOG_INTERVAL) {
      lastLog = now;
      Serial.print(now); Serial.print(",");
      Serial.print(data.temperature); Serial.print(",");
      Serial.print(data.humidity); Serial.print(",");
      Serial.println(status);
    }
  }
}
