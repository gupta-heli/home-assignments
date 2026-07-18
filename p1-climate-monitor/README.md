# Project 1: Smart Room Climate Monitor

A live temperature and humidity monitor using ESP32 that displays sensor readings on an OLED screen, logs data to Serial in CSV format, and triggers an active buzzer + Red LED alert when temperature exceeds 38°C or humidity exceeds 80%.

## Components Used
- **ESP32 Dev Board**
- **DHT22 Temperature & Humidity Sensor** (AM2302)
- **OLED Display 0.96"** — 7-pin SPI SSD1306 module
- **Active Buzzer**
- **LEDs**: Red LED (Alert) & Green LED (Normal Status)
- **Resistors**: 220Ω (×2 for LEDs), 10kΩ pull-up for DHT22 DATA pin
- **Breadboard & Jumper Wires**

## ⚠️ Hardware Notes
- **OLED**: Kit sheet specifies an I2C OLED (4-pin). The hardware module supplied is the **7-pin SPI SSD1306** (GND, VCC, D0, D1, RES, DC, CS). This implementation uses `Adafruit_SSD1306` in SPI mode. If using a 4-pin I2C module, update constructor to `Adafruit_SSD1306(128, 64, &Wire, -1)` with SDA→GPIO21 and SCL→GPIO22.
- **Temp/Humidity Sensor**: Kit sheet specifies DHT11; the supplied sensor is a **DHT22** (AM2302). The code is configured for `DHTesp::DHT22`. If using a DHT11 sensor, change the setup line to `dht.setup(4, DHTesp::DHT11)`.

## Wiring Summary / Wiring Diagram

```text
[DHT22 Sensor]
  VCC  --------------> 3.3V
  GND  --------------> GND
  DATA --------------> GPIO4 (with 10kΩ pull-up resistor to 3.3V)

[OLED Display (SPI SSD1306 7-Pin)]
  VCC  --------------> 3.3V (DO NOT CONNECT TO 5V)
  GND  --------------> GND
  D0 (SCK/CLK) ------> GPIO18 (Hardware SPI Clock)
  D1 (MOSI/DATA) ----> GPIO23 (Hardware SPI Data)
  RES (Reset) -------> GPIO16
  DC (Data/Command) -> GPIO17
  CS (Chip Select) --> GPIO5

[Alert Outputs]
  Red LED Anode  ----> GPIO25 ---> 220Ω resistor ---> Red LED Anode ---> GND
  Green LED Anode ---> GPIO26 ---> 220Ω resistor ---> Green LED Anode -> GND
  Buzzer Positive ---> GPIO27 ---> GND
```

## Library Versions
- `DHTesp` (v1.18.0) by beegee-tokyo
- `Adafruit_SSD1306` (v2.5.7)
- `Adafruit_GFX` Library (v1.11.5)

## How to Run
1. Open Arduino IDE and install the required libraries (`DHTesp`, `Adafruit_SSD1306`, `Adafruit_GFX`).
2. Connect hardware components according to the wiring diagram above.
3. Select Board: **ESP32 Dev Module**.
4. Open `p1_climate_monitor/p1_climate_monitor.ino` and upload to your ESP32 board.
5. Open Serial Monitor set to **115200 baud rate**.

## Expected Output
- **OLED Display**: Shows live Temperature, Humidity, and Comfort Status (`COOL`, `COMFORT`, `HOT`, or `DANGER`). Screen automatically alternates every 5 seconds with the Daily Max/Min Temperature page.
- **Alert System**:
  - Normal condition (Temp ≤ 38°C and Humidity ≤ 80%): Green LED remains ON.
  - Alert condition (Temp > 38°C or Humidity > 80%): Red LED turns ON and Active Buzzer beeps for 1 second.
- **Serial Monitor**: Logs CSV formatted readings every 5 seconds:
  `millis(),temp,humidity,status`

## Bonus Challenge Implemented
Includes a daily max/min temperature tracker using `millis()`. Highest and lowest temperatures recorded since power-on are stored and displayed on a dedicated third OLED screen cycling every 5 seconds.

## Demo Video
- [Demo Video Google Drive Link](https://drive.google.com/file/d/1vLFONvlPQga3nKYxwgaMuzZK6_eY-hea/view?usp=drivesdk)
