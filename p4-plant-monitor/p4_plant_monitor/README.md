# Project 4: Smart Plant Watering Monitor

An automated plant watering system using an ESP32 that monitors soil moisture and ambient temperature, displays readings on an OLED screen with a moisture bar graph, and automatically controls a water pump relay using hysteresis logic. A manual override button allows forcing a 5-second watering cycle at any time.

## Components Used
- **ESP32 Dev Board**
- **Capacitive Soil Moisture Sensor** (3.3V compatible)
- **DHT22 Temperature & Humidity Sensor** (AM2302)
- **2-Channel Relay Module** (active-LOW, 5V coil)
- **OLED Display 0.96"** — 7-pin SPI SSD1306 module (adapted from kit sheet I2C spec)
- **Push Button** — manual watering override (uses built-in boot button GPIO0)
- **Breadboard & Jumper Wires**

## ⚠️ Hardware Notes
- **OLED**: The kit sheet specifies an I2C OLED (4-pin). The supplied module is the **7-pin SPI SSD1306**. This build uses hardware SPI (SCK=GPIO18, MOSI=GPIO23). For I2C, swap the constructor to `Adafruit_SSD1306(128, 64, &Wire, -1)` with SDA→GPIO21 and SCL→GPIO22.
- **DHT Sensor**: The kit sheet specifies DHT11, but the supplied sensor is a **DHT22** (AM2302). The sketch is configured to `DHTesp::DHT22`. If using a DHT11, change this one constant.
- **Soil Sensor Voltage**: The capacitive soil moisture sensor must be powered at **3.3V**, NOT 5V, to avoid damaging the ESP32 ADC input on GPIO35.

## Wiring Summary / Wiring Diagram

```text
[Capacitive Soil Moisture Sensor]
  VCC  --------------> 3.3V (NOT 5V)
  GND  --------------> GND
  AOUT --------------> GPIO35 (ADC input, input-only pin)

[DHT22 Sensor]
  VCC  --------------> 3.3V
  GND  --------------> GND
  DATA --------------> GPIO4 (with 10kΩ pull-up resistor to 3.3V)

[2-Channel Relay Module]
  VCC  --------------> 5V
  GND  --------------> GND
  IN1  --------------> GPIO26 (active-LOW: LOW = pump ON, HIGH = pump OFF)

[OLED Display (SPI SSD1306 7-Pin)]
  VCC  --------------> 3.3V
  GND  --------------> GND
  D0 (SCK/CLK) ------> GPIO18 (Hardware SPI Clock)
  D1 (MOSI/DATA) ----> GPIO23 (Hardware SPI Data)
  RES (Reset) -------> GPIO14
  DC (Data/Command) -> GPIO27
  CS (Chip Select) --> GPIO5

[Manual Override Button]
  One leg -------------> GPIO0 (INPUT_PULLUP / boot button)
  Other leg -----------> GND
```

## Libraries
- `DHTesp` (v1.18.0) by beegee-tokyo
- `Adafruit_SSD1306` (v2.5.7)
- `Adafruit_GFX` Library (v1.11.5)

## Sensor Calibration
The capacitive soil moisture sensor **must be calibrated** for accurate readings:
1. Place the sensor in **dry open air** → note the raw `analogRead(35)` value → set as `DRY_RAW`.
2. Submerge the sensor tip in **water up to the marked line** → note the raw value → set as `WET_RAW`.
3. Update `DRY_RAW` and `WET_RAW` constants at the top of the sketch.

> **Values used in this sketch**: `DRY_RAW = 2207`, `WET_RAW = 656` (measured with the supplied sensor). Replace with your own calibrated values.

## How to Run
1. Wire all components per the diagram above.
2. Calibrate the soil sensor and update `DRY_RAW` / `WET_RAW` constants.
3. Select Board: **ESP32 Dev Module** in the Arduino IDE.
4. Upload `p4_plant_monitor.ino`.
5. Open Serial Monitor at **115200 baud rate**.

## Expected Output
- **OLED Display**: Shows Soil Moisture %, Temperature in °C, status label (`DRY` / `OPTIMAL` / `WET`), and a visual moisture bar graph.
- **Automatic Pump Control** (hysteresis logic):
  - Pump **turns ON** when soil moisture drops below **30%**.
  - Pump **turns OFF** when soil moisture rises above **40%**.
  - The hysteresis gap (30%–40%) prevents rapid relay switching.
- **Manual Override Button** (GPIO0): Pressing the button forces the pump ON for **5 seconds**, then automatically resumes the auto-control mode.
- **Serial Monitor**: Logs soil %, temperature, soil status, and pump state every read cycle.

## Demo Video
- [Demo Video Google Drive Link](https://drive.google.com/file/d/1DveY83dGN6ks4dDuBE6NxnJa8Oq3XJ8J/view?usp=drivesdk)
