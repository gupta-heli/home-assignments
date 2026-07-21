# Project 10: Full IoT Home Automation Hub (Capstone)

A comprehensive smart home automation hub built on the ESP32 that integrates climate monitoring (DHT22), flammable gas detection (MQ-2), motion detection (PIR HC-SR501), ambient light sensing (LDR), automated dual-relay load control (Fan + Light), safety alarm indicators, local 3-page OLED display, an asynchronous web dashboard with manual control buttons, and MQTT publishing to `broker.hivemq.com`.

## Components Used
- **ESP32 Dev Board**
- **DHT22 Temperature & Humidity Sensor** (AM2302)
- **MQ-2 Flammable Gas & Smoke Sensor**
- **PIR Motion Sensor** (HC-SR501)
- **LDR Light Sensor** + 10kΩ resistor (voltage divider)
- **2-Channel Relay Module** (Fan + Light, active-LOW)
- **OLED Display 0.96"** — 7-pin SPI SSD1306 module (adapted from kit sheet I2C spec)
- **Active Buzzer**, Red LED (Gas Alert), Green LED (System OK)
- **Push Buttons ×2** (Manual Fan override, Manual Light override)
- **Breadboard & Jumper Wires**

## ⚠️ Hardware Notes
- **OLED**: The kit sheet lists an I2C OLED (4-pin). The supplied module is the **7-pin SPI SSD1306**, wired over hardware SPI (SCK=GPIO18, MOSI=GPIO23).
- **DHT Sensor**: Uses `DHTesp::DHT22` for AM2302 sensor timing.

## System Architecture

```text
[DHT22] [MQ-2] [PIR] [LDR]  --5s poll-->  readSensors()
                                     |
                                     v
                          applyAutomationRules()
                      (Gas Safety > Fan Rule > Light Rule)
                                     |
                    -----------------------------------
                    |                |                |
              [Relay: Fan]    [Relay: Light]   [Buzzer/RedLED]
                    |                |                |
                    -----------------------------------
                                     |
                +--------------------+--------------------+
                |                                         |
      AsyncWebServer (Port 80)                  MQTT Publish (30s)
      / dashboard, /data JSON,                   broker.hivemq.com
      /fan/toggle, /light/toggle                 topic: iitjammu/<user>/home
                                     |
                               OLED Display
                            (3 pages, 5s cycle)
```

## Automation Rules & Priority Matrix
1. **Gas Safety Override (Highest Priority)**: If MQ-2 gas concentration exceeds 60%, the active buzzer sounds, Red LED turns ON, and **both relays force OFF immediately** regardless of manual override state.
2. **Fan Automation (Relay 1)**: Turns AUTO ON when temperature > 33°C, and AUTO OFF when temperature < 28°C (hysteresis gap). Manual button/web toggle overrides auto state for 10 minutes.
3. **Smart Light Automation (Relay 2)**: Turns AUTO ON when ambient light < 20% AND PIR motion is detected. Turns AUTO OFF when light > 60% or no motion for 3 minutes. Manual override lasts 10 minutes.

## Wiring Reference

| Component        | Pin       | ESP32 GPIO |
|-------------------|----------|------------|
| DHT22 DATA        | Data     | GPIO4 (10kΩ pull-up to 3.3V) |
| MQ-2 AOUT         | Analog   | GPIO35 |
| PIR OUT           | Digital  | GPIO13 |
| LDR divider       | Analog   | GPIO34 |
| Relay IN1 (Fan)   | Control  | GPIO26 (active-LOW) |
| Relay IN2 (Light) | Control  | GPIO27 (active-LOW) |
| Buzzer            | +        | GPIO14 |
| Red LED (gas alert)| Anode   | GPIO25 → 220Ω → GND |
| Green LED (Sys OK)| Anode    | GPIO2 → 220Ω → GND |
| Fan override btn  | one leg  | GPIO0 (INPUT_PULLUP), other leg → GND |
| Light override btn| one leg  | GPIO32 (INPUT_PULLUP), other leg → GND |
| OLED D0 (SCK)     | Clock    | GPIO18 (Hardware SPI) |
| OLED D1 (MOSI)    | Data     | GPIO23 (Hardware SPI) |
| OLED RES          | Reset    | GPIO19 |
| OLED DC           | Data/Cmd | GPIO21 |
| OLED CS           | Chip Sel | GPIO5 |

## Libraries Required
- `WiFi` (ESP32 core)
- `ESPAsyncWebServer` & `AsyncTCP`
- `PubSubClient` (MQTT)
- `DHTesp`
- `Adafruit_SSD1306` & `Adafruit_GFX`

## Configuration Setup
1. Copy `config.h.example` to `config.h` in the `p10_home_hub` folder:
   ```cpp
   #define WIFI_SSID     "your_wifi_name"
   #define WIFI_PASSWORD "your_wifi_password"
   #define MQTT_USERNAME "yourname" // Topic: iitjammu/<yourname>/home
   ```
2. `config.h` is gitignored to keep credentials secure.

## How to Run
1. Wire components according to the pinout table.
2. Edit `config.h` with your Wi-Fi credentials and username.
3. Open `p10_home_hub/p10_home_hub.ino` in Arduino IDE.
4. Select Board: **ESP32 Dev Module**.
5. Upload and open Serial Monitor at **115200 baud** to view IP address.

## Expected Output
- **Local OLED Display**: Cycles 3 status pages every 5s:
  - Page 1: Temp & Humidity (`T: xx.xC H: xx%`)
  - Page 2: Gas %, PIR state, Light % (`Gas: xx% PIR: MOTION Light: xx%`)
  - Page 3: Relay states & System Uptime (`Fan: ON Light: OFF 0d 0h 5m`)
- **Web Dashboard (`http://<ESP32-IP>/`)**: Real-time HTTP dashboard with Fan and Light manual toggle buttons.
- **JSON API (`http://<ESP32-IP>/data`)**: Returns JSON:
  ```json
  {"temp": 34.2, "humidity": 55.0, "gas": 12, "pir": true, "light": 15, "fan": true, "light_relay": true, "alert": false}
  ```
- **MQTT Broker Publishing**: Publishes JSON payload every 30 seconds to `iitjammu/<MQTT_USERNAME>/home` on `broker.hivemq.com:1883`.

## Demo Video
- [Demo Video Google Drive Link](https://drive.google.com/file/d/1ZG5S2_FP5dle4uLXW0F3x9J1LTlG_Mzw/view?usp=drivesdk)
