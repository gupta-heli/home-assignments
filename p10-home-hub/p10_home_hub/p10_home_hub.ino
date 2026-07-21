/*
  Project 10: Full IoT Home Automation Hub (Capstone)
  Board: ESP32 Dev Board
  NOTE: OLED is the kit's actual 7-pin SPI SSD1306 module.
*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h" // WIFI_SSID, WIFI_PASSWORD, MQTT_USERNAME

#define DHT_PIN     4
#define MQ2_PIN     35
#define PIR_PIN     13
#define LDR_PIN     34
#define RELAY_FAN   26
#define RELAY_LIGHT 27
#define BUZZER_PIN  14
#define RED_LED     25   // gas alert
#define GREEN_LED   2    // system OK
#define FAN_BTN     0
#define LIGHT_BTN   32

#define OLED_CS    5
#define OLED_DC    21
#define OLED_RESET 19
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

DHTesp dht;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
AsyncWebServer server(80);

float temperature = 0, humidity = 0;
int gasPercent = 0, lightPercent = 0;
bool pirMotion = false;
bool fanOn = false, lightOn = false;
bool fanManual = false, lightManual = false;
unsigned long fanManualUntil = 0, lightManualUntil = 0;
unsigned long lastMotionTime = 0;
unsigned long lastSensorRead = 0;
unsigned long lastMqttPublish = 0;
unsigned long bootTime = 0;
int oledPage = 0;
unsigned long lastPageSwitch = 0;

void applyRelays() {
  digitalWrite(RELAY_FAN, fanOn ? LOW : HIGH);     // active-LOW
  digitalWrite(RELAY_LIGHT, lightOn ? LOW : HIGH);
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
}

void connectMQTT() {
  if (mqttClient.connected()) return;
  String clientId = "esp32-home-hub-" + String(random(0xffff), HEX);
  mqttClient.connect(clientId.c_str());
}

String buildJson() {
  String j = "{";
  j += "\"temp\":" + String(temperature, 1) + ",";
  j += "\"humidity\":" + String(humidity, 1) + ",";
  j += "\"gas\":" + String(gasPercent) + ",";
  j += "\"pir\":" + String(pirMotion ? "true" : "false") + ",";
  j += "\"light\":" + String(lightPercent) + ",";
  j += "\"fan\":" + String(fanOn ? "true" : "false") + ",";
  j += "\"light_relay\":" + String(lightOn ? "true" : "false") + ",";
  j += "\"alert\":" + String(gasPercent > 60 ? "true" : "false");
  j += "}";
  return j;
}

String buildHtmlPage() {
  String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='10'>";
  html += "<title>Home Automation Hub</title>";
  html += "<style>body{font-family:sans-serif;text-align:center;padding:20px;background:#eef2f5;}";
  html += ".card{background:white;border-radius:10px;padding:14px;margin:8px auto;max-width:320px;box-shadow:0 2px 5px rgba(0,0,0,0.15);}";
  html += "button{padding:8px 16px;margin:4px;border-radius:6px;border:none;background:#3498db;color:white;}</style></head><body>";
  html += "<h2>Home Automation Hub</h2>";
  html += "<div class='card'>Temp: " + String(temperature, 1) + " C | Hum: " + String(humidity, 1) + " %</div>";
  html += "<div class='card'>Gas: " + String(gasPercent) + " % " + (gasPercent > 60 ? "⚠️ ALERT" : "") + "</div>";
  html += "<div class='card'>PIR: " + String(pirMotion ? "MOTION" : "clear") + " | Light: " + String(lightPercent) + "%</div>";
  html += "<div class='card'>Fan: " + String(fanOn ? "ON" : "OFF") + " <a href='/fan/toggle'><button>Toggle</button></a></div>";
  html += "<div class='card'>Light: " + String(lightOn ? "ON" : "OFF") + " <a href='/light/toggle'><button>Toggle</button></a></div>";
  unsigned long up = (millis() - bootTime) / 1000;
  html += "<div class='card'>Uptime: " + String(up / 86400) + "d " + String((up % 86400) / 3600) + "h " + String((up % 3600) / 60) + "m</div>";
  html += "</body></html>";
  return html;
}

void readSensors() {
  TempAndHumidity data = dht.getTempAndHumidity();
  temperature = data.temperature;
  humidity = data.humidity;
  gasPercent = map(analogRead(MQ2_PIN), 0, 4095, 0, 100);
  lightPercent = map(analogRead(LDR_PIN), 0, 4095, 0, 100);
  pirMotion = digitalRead(PIR_PIN) == HIGH;
  if (pirMotion) lastMotionTime = millis();
}

void applyAutomationRules() {
  if (fanManual && millis() > fanManualUntil) fanManual = false;
  if (lightManual && millis() > lightManualUntil) lightManual = false;

  // Rule 3: Gas safety shutdown takes priority over everything
  if (gasPercent > 60) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RED_LED, HIGH);
    fanOn = false; lightOn = false;
    applyRelays();
    return;
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED, LOW);
  }

  // Rule 1: Fan hysteresis by temperature (skip if manual override active)
  if (!fanManual) {
    if (!fanOn && temperature > 33) fanOn = true;
    else if (fanOn && temperature < 28) fanOn = false;
  }

  // Rule 2: Light by LDR + PIR (skip if manual override active)
  if (!lightManual) {
    bool dark = lightPercent < 20;
    bool noMotionFor3Min = (millis() - lastMotionTime) > 3UL * 60UL * 1000UL;
    if (!lightOn && dark && pirMotion) lightOn = true;
    else if (lightOn && (lightPercent > 60 || noMotionFor3Min)) lightOn = false;
  }

  applyRelays();
}

void publishMQTT() {
  if (!mqttClient.connected()) connectMQTT();
  mqttClient.loop();
  String topic = String("iitjammu/") + MQTT_USERNAME + "/home";
  mqttClient.publish(topic.c_str(), buildJson().c_str());
}

void updateOLED() {
  static bool firstRun = true;
  if (!firstRun && millis() - lastPageSwitch < 5000) return;
  if (firstRun) {
    firstRun = false;
    lastPageSwitch = millis();
  } else {
    lastPageSwitch = millis();
    oledPage = (oledPage + 1) % 3;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (oledPage == 0) {
    display.println("Temp/Humidity");
    display.print("T: "); display.print(temperature, 1); display.println(" C");
    display.print("H: "); display.print(humidity, 1); display.println(" %");
  } else if (oledPage == 1) {
    display.println("Gas/PIR/Light");
    display.print("Gas: "); display.print(gasPercent); display.println(" %");
    display.print("PIR: "); display.println(pirMotion ? "MOTION" : "clear");
    display.print("Light: "); display.print(lightPercent); display.println(" %");
  } else {
    unsigned long up = (millis() - bootTime) / 1000;
    display.println("Relays/Uptime");
    display.print("Fan: "); display.println(fanOn ? "ON" : "OFF");
    display.print("Light: "); display.println(lightOn ? "ON" : "OFF");
    display.print(up / 86400); display.print("d ");
    display.print((up % 86400) / 3600); display.print("h ");
    display.print((up % 3600) / 60); display.println("m");
  }
  display.display();
}

void setup() {
  Serial.begin(115200);
  bootTime = millis();

  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(FAN_BTN, INPUT_PULLUP);
  pinMode(LIGHT_BTN, INPUT_PULLUP);
  applyRelays();
  digitalWrite(GREEN_LED, HIGH);

  dht.setup(DHT_PIN, DHTesp::DHT22);
  analogReadResolution(12);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED init failed");
  }
  display.clearDisplay();
  display.display();

  connectWiFi();
  mqttClient.setServer("broker.hivemq.com", 1883);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html", buildHtmlPage());
  });
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", buildJson());
  });
  server.on("/fan/toggle", HTTP_GET, [](AsyncWebServerRequest *req) {
    fanOn = !fanOn; fanManual = true; fanManualUntil = millis() + 10UL * 60UL * 1000UL;
    applyRelays();
    req->redirect("/");
  });
  server.on("/light/toggle", HTTP_GET, [](AsyncWebServerRequest *req) {
    lightOn = !lightOn; lightManual = true; lightManualUntil = millis() + 10UL * 60UL * 1000UL;
    applyRelays();
    req->redirect("/");
  });
  server.begin();

  // Force initial sensor reading and automation rule check on boot
  readSensors();
  applyAutomationRules();
}

void loop() {
  // manual button overrides (edge-triggered, active-low)
  static bool lastFanBtn = HIGH, lastLightBtn = HIGH;
  bool fanBtn = digitalRead(FAN_BTN);
  bool lightBtn = digitalRead(LIGHT_BTN);
  if (fanBtn == LOW && lastFanBtn == HIGH) {
    fanOn = !fanOn; fanManual = true; fanManualUntil = millis() + 10UL * 60UL * 1000UL;
    applyRelays();
  }
  if (lightBtn == LOW && lastLightBtn == HIGH) {
    lightOn = !lightOn; lightManual = true; lightManualUntil = millis() + 10UL * 60UL * 1000UL;
    applyRelays();
  }
  lastFanBtn = fanBtn; lastLightBtn = lightBtn;

  if (millis() - lastSensorRead > 5000) {
    lastSensorRead = millis();
    readSensors();
    applyAutomationRules();
  }

  if (millis() - lastMqttPublish > 30000) {
    lastMqttPublish = millis();
    publishMQTT();
  }

  updateOLED();
}
