/*
  Project 6: Wi-Fi Weather Dashboard
  Board: ESP32 Dev Board
  NOTE: OLED is the kit's actual 7-pin SPI SSD1306 (not I2C as in the sheet).
  BMP280 stays on I2C (SDA=21, SCL=22) as usual - only the display moved to SPI.
*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <DHTesp.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"  // defines WIFI_SSID and WIFI_PASSWORD

#define DHT_PIN   4
#define LDR_PIN   34

// SPI OLED
#define OLED_CS    5
#define OLED_DC    32
#define OLED_RESET 33
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

Adafruit_BMP280 bmp;
bool bmpOK = false;
DHTesp dht;
AsyncWebServer server(80);

float temperature = 0, humidity = 0, pressure = 0, altitude = 0;
int lightPercent = 0;
unsigned long lastRead = 0;

String buildHtmlPage() {
  String bg = (temperature > 32) ? "#f4b183" : "#a9cce3";
  String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='10'>";
  html += "<title>ESP32 Weather Dashboard</title>";
  html += "<style>body{font-family:sans-serif;background:" + bg + ";text-align:center;padding:20px;}";
  html += ".card{background:white;border-radius:12px;padding:16px;margin:10px auto;max-width:300px;box-shadow:0 2px 6px rgba(0,0,0,0.2);}</style></head><body>";
  html += "<h2>ESP32 Weather Dashboard</h2>";
  html += "<div class='card'>Temperature: " + String(temperature, 1) + " &deg;C</div>";
  html += "<div class='card'>Humidity: " + String(humidity, 1) + " %</div>";
  html += "<div class='card'>Pressure: " + String(pressure, 1) + " hPa</div>";
  html += "<div class='card'>Altitude: " + String(altitude, 1) + " m</div>";
  html += "<div class='card'>Light: " + String(lightPercent) + " %</div>";
  html += "</body></html>";
  return html;
}

String buildJson() {
  String j = "{";
  j += "\"temp\":" + String(temperature, 1) + ",";
  j += "\"humidity\":" + String(humidity, 1) + ",";
  j += "\"pressure\":" + String(pressure, 1) + ",";
  j += "\"altitude\":" + String(altitude, 1) + ",";
  j += "\"light\":" + String(lightPercent);
  j += "}";
  return j;
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected. IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connect failed.");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  bmpOK = bmp.begin(0x76);
  if (!bmpOK) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }

  dht.setup(DHT_PIN, DHTesp::DHT22);
  analogReadResolution(12);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED init failed");
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();

  connectWiFi();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("SSID: " + String(WIFI_SSID));
  display.println("IP: " + (WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : String("N/A")));
  display.display();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html", buildHtmlPage());
  });
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", buildJson());
  });
  server.begin();
}

void loop() {
  if (millis() - lastRead > 5000) {
    lastRead = millis();
    TempAndHumidity data = dht.getTempAndHumidity();
    temperature = data.temperature;
    humidity = data.humidity;
    if (bmpOK) {
      pressure = bmp.readPressure() / 100.0F;
      altitude = bmp.readAltitude(1013.25);
    } else {
      pressure = 0.0;
      altitude = 0.0;
    }
    int raw = analogRead(LDR_PIN);
    lightPercent = map(raw, 0, 4095, 0, 100);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("T:"); display.print(temperature, 1); display.print("C H:"); display.println(humidity, 0);
    display.setCursor(0, 12);
    display.print("P:"); display.print(pressure, 0); display.println("hPa");
    display.setCursor(0, 24);
    display.print("Alt:"); display.print(altitude, 0); display.println("m");
    display.setCursor(0, 36);
    display.print("Light:"); display.print(lightPercent); display.println("%");
    display.setCursor(0, 48);
    display.print("IP:"); display.println(WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : String("N/A"));
    display.display();
  }
}
