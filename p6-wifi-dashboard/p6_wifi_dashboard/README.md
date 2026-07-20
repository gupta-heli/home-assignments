# Project 6: Wi-Fi Weather Dashboard

An IoT weather station built using an ESP32. It reads environmental parameters (temperature, humidity, atmospheric pressure, estimated altitude, and ambient light level), displays them locally on a 0.96" SPI OLED display, and hosts an asynchronous web server serving a live web dashboard (`/`) and a JSON endpoint (`/data`).

## Components Used
- **ESP32 Dev Board**
- **DHT22 Temperature & Humidity Sensor** (AM2302)
- **BMP280 Barometric Pressure Sensor** (I2C)
- **LDR Light Sensor** + 10kΩ resistor (voltage divider)
- **OLED Display 0.96"** — 7-pin SPI SSD1306 module (adapted from kit sheet I2C spec)
- **Breadboard & Jumper Wires**

## ⚠️ Hardware Notes
- **BMP280**: Connected over I2C at address `0x76` (SDA=GPIO21, SCL=GPIO22).
- **OLED Display**: Kit sheet lists an I2C OLED, but the supplied module is the **7-pin SPI SSD1306**. Connected via hardware SPI (SCK=GPIO18, MOSI=GPIO23).
- **DHT Sensor**: Configured for `DHTesp::DHT22`.

## Wiring Summary / Wiring Diagram

```text
[BMP280 Pressure Sensor (I2C)]
  VCC  --------------> 3.3V
  GND  --------------> GND
  SDA  --------------> GPIO21 (I2C SDA)
  SCL  --------------> GPIO22 (I2C SCL)

[DHT22 Sensor]
  VCC  --------------> 3.3V
  GND  --------------> GND
  DATA --------------> GPIO4 (with 10kΩ pull-up to 3.3V)

[LDR Light Sensor (Voltage Divider)]
  3.3V ---> LDR ---> GPIO34 (ADC input) ---> 10kΩ resistor ---> GND

[OLED Display (SPI SSD1306 7-Pin)]
  VCC  --------------> 3.3V
  GND  --------------> GND
  D0 (SCK/CLK) ------> GPIO18 (Hardware SPI Clock)
  D1 (MOSI/DATA) ----> GPIO23 (Hardware SPI Data)
  RES (Reset) -------> GPIO33
  DC (Data/Command) -> GPIO32
  CS (Chip Select) --> GPIO5
```

## Libraries Required
- `WiFi` (ESP32 Built-in)
- `ESPAsyncWebServer` & `AsyncTCP`
- `Adafruit_BMP280`
- `DHTesp`
- `Adafruit_SSD1306` & `Adafruit_GFX`

## Configuration Setup
1. Copy `config.h.example` to `config.h` in the `p6_wifi_dashboard` directory:
   ```cpp
   #define WIFI_SSID "GV Supernet 5"
   #define WIFI_PASSWORD "Madhu9100"
   ```
2. `config.h` is excluded from git tracking via `.gitignore` to prevent committing sensitive network credentials.

## How to Run
1. Wire components per the diagram above.
2. Configure `config.h` with your local Wi-Fi credentials.
3. Open `p6_wifi_dashboard.ino` in Arduino IDE.
4. Board: **ESP32 Dev Module**.
5. Upload and open Serial Monitor at **115200 baud** to get the assigned local IP address.

## Expected Output
- **OLED Display**: Shows Wi-Fi SSID and local IP address during connection, followed by real-time updates for Temperature, Humidity, Pressure, Altitude, Light %, and IP.
- **Web Dashboard (`http://<ESP32-IP>/`)**: Serves an auto-refreshing (every 10s) HTML weather dashboard with dynamic background color based on temperature.
- **JSON API Endpoint (`http://<ESP32-IP>/data`)**: Returns current readings in JSON format:
  ```json
  {"temp": 28.5, "humidity": 65.0, "pressure": 1012.3, "altitude": 120.5, "light": 82}
  ```
- **Wi-Fi Auto-Reconnection**: Background health check retries connection every 30 seconds if Wi-Fi drops.

## Demo Video
- [Demo Video Google Drive Link](https://drive.google.com/file/d/15fWOxQ6lwCGY5wamdTr4Z5B-RtmRquOx/view?usp=drivesdk)
