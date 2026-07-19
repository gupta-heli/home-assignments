/*
  Project 4: Smart Plant Watering Monitor
  Board: ESP32 Dev Board
  Components: Capacitive Soil Sensor, DHT22, Relay Module, SPI OLED, Push Button
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>

#define SOIL_PIN     35
#define DHT_PIN      4
#define RELAY1_PIN   26   // pump relay (active-LOW)
#define BUTTON_PIN   0    // manual override (boot button, active-low)

#define PUMP_ON      LOW  // active-LOW relay module
#define PUMP_OFF     HIGH

// SPI OLED (kit's actual 7-pin SSD1306, hardware SPI SCK=18, MOSI=23)
#define OLED_CS      5
#define OLED_DC      27
#define OLED_RESET   14
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Calibration — measure for YOUR sensor and update these values
int DRY_RAW = 2207;
int WET_RAW = 656;

DHTesp dht;
bool pumpOn = false;
bool manualWater = false;
unsigned long manualStart = 0;
unsigned long lastRead = 0;
const unsigned long READ_INTERVAL = 5UL * 1000UL; // 5 seconds for testing

void setup() {
  Serial.begin(115200);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(RELAY1_PIN, PUMP_OFF);
  dht.setup(DHT_PIN, DHTesp::DHT22);
  analogReadResolution(12);
  pinMode(SOIL_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED init failed");
  }
  display.clearDisplay();
  display.display();
  lastRead = millis() - READ_INTERVAL; // force first reading immediately
}

void drawOLED(int moisturePercent, float temp, String status) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Soil: "); display.print(moisturePercent); display.println("%");
  display.setCursor(0, 12);
  display.print("Temp: "); display.print(temp, 1); display.println("C");
  display.setCursor(0, 24);
  display.print("Status: "); display.println(status);
  int barLen = map(moisturePercent, 0, 100, 0, 120);
  display.drawRect(0, 40, 122, 10, SSD1306_WHITE);
  display.fillRect(1, 41, barLen, 8, SSD1306_WHITE);
  display.display();
}

void loop() {
  // Manual override button (forces pump ON for 5 seconds)
  if (digitalRead(BUTTON_PIN) == LOW && !manualWater) {
    manualWater = true;
    manualStart = millis();
    digitalWrite(RELAY1_PIN, PUMP_ON);
    Serial.println("Manual override: forced watering");
  }
  if (manualWater && millis() - manualStart > 5000) {
    manualWater = false;
    digitalWrite(RELAY1_PIN, pumpOn ? PUMP_ON : PUMP_OFF);
  }

  if (millis() - lastRead >= READ_INTERVAL) {
    lastRead = millis();

    int raw = analogRead(SOIL_PIN);
    int moisturePercent = constrain(map(raw, DRY_RAW, WET_RAW, 0, 100), 0, 100);

    TempAndHumidity data = dht.getTempAndHumidity();
    if (dht.getStatus() != DHTesp::ERROR_NONE) {
      Serial.print("DHT22 error: ");
      Serial.println(dht.getStatusString());
    }

    String soilStatus = moisturePercent < 30 ? "DRY" : (moisturePercent > 70 ? "WET" : "OPTIMAL");

    // Hysteresis: pump ON below 30%, OFF above 40%
    if (!pumpOn && moisturePercent < 30) {
      pumpOn = true;
      if (!manualWater) digitalWrite(RELAY1_PIN, PUMP_ON);
    } else if (pumpOn && moisturePercent > 40) {
      pumpOn = false;
      if (!manualWater) digitalWrite(RELAY1_PIN, PUMP_OFF);
    }

    Serial.print("Soil: "); Serial.print(moisturePercent); Serial.print("% | ");
    Serial.print("Temp: "); Serial.print(data.temperature); Serial.print("C | ");
    Serial.print("Status: "); Serial.print(soilStatus);
    Serial.print(" | Pump: "); Serial.println(pumpOn ? "ON" : "OFF");

    drawOLED(moisturePercent, data.temperature, soilStatus);
  }
}
