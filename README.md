# 🎓 IIT Jammu Summer School 2026 — IoT & Drones Home Assignments Submission

<div align="center">

  ![IIT Jammu IoT & Drones](https://img.shields.io/badge/IIT%20Jammu-Summer%20School%202026-blueviolet?style=for-the-badge&logo=education)
  ![Author](https://img.shields.io/badge/Author-Heli%20Gupta-orange?style=for-the-badge&logo=github)
  ![Hardware](https://img.shields.io/badge/Hardware-ESP32%20%7C%20Arduino%20Uno-00979D?style=for-the-badge&logo=arduino)
  ![Status](https://img.shields.io/badge/Status-Completed%20%26%20Submitted-success?style=for-the-badge&logo=checkmarx)
  ![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

  <h3>⚡ Complete Multi-Sensor Embedded Systems, Web Dashboards & Capstone Home Automation Hub ⚡</h3>

  <p align="center">
    <a href="#-projects-index"><b>Explore Projects</b></a> •
    <a href="#-capstone-home-automation-hub-p10"><b>Capstone Hub</b></a> •
    <a href="#-hardware--component-matrix"><b>Hardware Setup</b></a> •
    <a href="#-technical-innovations--engineering-highlights"><b>Engineering Highlights</b></a> •
    <a href="./DOCUMENTATION.md"><b>Full Documentation Report</b></a>
  </p>
</div>

---

## 📌 Submission Overview

This repository contains the official home assignment submissions for the **IIT Jammu Summer School 2026 (IoT & Drones Program)** developed by **Heli Gupta**.

Out of the 10 coursework assignments, **7 comprehensive core and capstone projects** (`P1`, `P2`, `P3`, `P4`, `P6`, `P9`, and the advanced `P10` Capstone Hub) have been engineered, optimized, hardware-adapted, fully documented, and submitted with complete demonstration videos.

> [!NOTE]  
> **Coursework Focus**: Projects 5 (Bluetooth Light Controller) and 7 (Motion Security Camera) were intentionally omitted to concentrate engineering effort on building, debugging, and perfecting the 7 complex multi-sensor, web-connected, and capstone automation modules.

---

## 📂 Projects Index

| # | Project Name | Microcontroller | Key Hardware & Sensors | Primary Logic & Features | Demo Video |
|:---:|---|:---:|---|---|:---:|
| **P1** | [**Smart Room Climate Monitor**](#-p1-smart-room-climate-monitor) | **ESP32** | DHT22, 7-pin SPI OLED, Active Buzzer, Red/Green LEDs | CSV Serial logging, Max/Min daily tracker, High-temp alert (>38°C) | [🎬 Watch Demo](https://drive.google.com/file/d/1vLFONvlPQga3nKYxwgaMuzZK6_eY-hea/view?usp=drivesdk) |
| **P2** | [**Gas & Fire Safety Alert System**](#-p2-gas--fire-safety-alert-system) | **ESP32** | MQ-2 Gas, Active-Low Flame Sensor, SPI OLED, Buzzer, Mute Btn | Gas (>40%) & Fire alarms, 60s non-blocking audio mute override | [🎬 Watch Demo](https://drive.google.com/file/d/1r8D-ZYj0fd1-MBPzozhY0dkahqsEN-Ib/view?usp=drivesdk) |
| **P3** | [**Ultrasonic Parking Assistant**](#-p3-ultrasonic-parking-assistant) | **Arduino Uno** | HC-SR04 Ultrasonic, SPI OLED, Active PWM Buzzer, Tri-LEDs | 5-sample acoustic filtering, 4 proximity zones with dynamic beep frequency | [🎬 Watch Demo](https://drive.google.com/file/d/1BDpT4dZ_QZiptEosnnRhm9D1HPPy0j5Y/view?usp=drivesdk) |
| **P4** | [**Smart Plant Watering Monitor**](#-p4-smart-plant-watering-monitor) | **ESP32** | Capacitive Soil Moisture, DHT22, 2-Ch Relay, SPI OLED, Button | Hysteresis pump control (ON <30%, OFF >40%), calibrated raw moisture scale | [🎬 Watch Demo](https://drive.google.com/file/d/1DveY83dGN6ks4dDuBE6NxnJa8Oq3XJ8J/view?usp=drivesdk) |
| **P6** | [**Wi-Fi Weather Dashboard**](#-p6-wi-fi-weather-dashboard) | **ESP32** | DHT22, BMP280 (I2C), LDR, SPI OLED, AsyncWebServer | Hosted HTML weather dashboard, `/data` REST JSON endpoint, auto Wi-Fi reconnect | [🎬 Watch Demo](https://drive.google.com/file/d/15fWOxQ6lwCGY5wamdTr4Z5B-RtmRquOx/view?usp=drivesdk) |
| **P9** | [**Stepper Precision Positioner**](#-p9-stepper-motor-precision-positioner) | **Arduino Uno** | 28BYJ-48 Stepper + ULN2003, 10k Potentiometer, Buttons, SPI OLED | 2048 step resolution (~0.176°/step), real-time pot target tracking, homing routine | [🎬 Watch Demo](https://drive.google.com/file/d/1s1UZSu1eKSoa8EFJN89dSSXOmbYgaI4E/view?usp=drivesdk) |
| **P10**| [**Full Home Automation Hub (Capstone)**](#-p10-full-iot-home-automation-hub-capstone) | **ESP32** | DHT22, MQ-2, PIR, LDR, 2-Ch Relay, Active Buzzer, OLED, Web Server, MQTT | Multi-sensor hub, safety-over-manual priority engine, web controls, HiveMQ MQTT cloud | [🎬 Watch Demo](https://drive.google.com/file/d/1ZG5S2_FP5dle4uLXW0F3x9J1LTlG_Mzw/view?usp=drivesdk) |

---

## 🏆 Capstone: Full IoT Home Automation Hub (P10)

The **Capstone Project (P10)** integrates climate monitoring, hazardous gas safety, motion detection, ambient light sensing, automated load switching, responsive web interfaces, and cloud telemetry into a unified embedded system.

```mermaid
flowchart TD
    subgraph Sensors ["Sensor Inputs (5s Polling Cycle)"]
        DHT[DHT22: Temp & Humidity]
        MQ2[MQ-2: Flammable Gas]
        PIR[PIR HC-SR501: Motion]
        LDR[LDR: Ambient Light]
    end

    subgraph Core ["ESP32 Automation Priority Engine"]
        Engine{Automation Priority Matrix}
        GasRule["Rule 1: Gas Safety (Highest Priority)\nIf Gas > 60% -> FORCE ALL RELAYS OFF"]
        FanRule["Rule 2: Climate Fan Control\nTemp > 33°C (ON) / Temp < 28°C (OFF)"]
        LightRule["Rule 3: Motion Lighting\nLight < 20% + PIR Motion -> Light ON"]
    end

    subgraph Outputs ["Local & Remote Outputs"]
        Relay1["2-Ch Relay: Fan (GPIO26)"]
        Relay2["2-Ch Relay: Light (GPIO27)"]
        Alarm["Buzzer (GPIO14) & Red LED (GPIO25)"]
        OLED["7-Pin SPI OLED Display (3 Pages)"]
        Web["AsyncWebServer (Port 80)\nDashboard & REST /data JSON"]
        Cloud["MQTT Cloud Publisher\nbroker.hivemq.com (Every 30s)"]
    end

    DHT --> Engine
    MQ2 --> Engine
    PIR --> Engine
    LDR --> Engine

    Engine --> GasRule
    Engine --> FanRule
    Engine --> LightRule

    GasRule --> Relay1
    GasRule --> Relay2
    GasRule --> Alarm

    FanRule --> Relay1
    LightRule --> Relay2

    Engine --> OLED
    Engine --> Web
    Engine --> Cloud
```

### ⚡ Priority Engine Logic
1. **Safety Emergency (Highest Priority)**: If MQ-2 gas concentration exceeds 60%, the active buzzer sounds, Red LED turns ON, and **both relays force OFF immediately**, overriding manual web and physical button toggles.
2. **Fan Control**: Automated ON when Temperature > 33°C, OFF when Temperature < 28°C. Manual override allows a 10-minute operation timer.
3. **Smart Lighting**: Automated ON when Ambient Light < 20% **and** Motion is detected by PIR. Automated OFF when Light > 60% or motion is inactive for 3 minutes.

---

## 🔬 Detailed Project Spotlights

<details>
<summary><b>🌡️ Project 1: Smart Room Climate Monitor (ESP32)</b> — <i>Click to expand</i></summary>

### Overview
Monitors ambient indoor climate conditions, cycling between live sensor telemetry and daily maximum/minimum statistics on a 7-pin SPI OLED screen while generating serial CSV logs and threshold alarms.

* **Alert Conditions**: Triggers continuous buzzer alert and Red LED when Temperature > 38°C or Humidity > 80%.
* **Daily Tracker**: Stores min/max temperatures recorded since boot using non-blocking `millis()` timing.
* **Serial CSV Output**: Logs formatted strings every 5 seconds: `timestamp,temp,humidity,status`.

```text
[DHT22] ---> GPIO4 (10kΩ Pull-Up)
[Buzzer] --> GPIO27
[Red LED] -> GPIO25 (Alert)
[Green] ---> GPIO26 (Safe)
[OLED] ----> Hardware SPI (CLK:18, MOSI:23, CS:5, DC:17, RES:16)
```
</details>

<details>
<summary><b>🚨 Project 2: Dual-Sensor Gas & Fire Safety System (ESP32)</b> — <i>Click to expand</i></summary>

### Overview
Integrated hazard monitoring combining an MQ-2 analog gas sensor and a digital active-low infrared flame phototransistor.

* **Multi-Stage Alarm**:
  * `SAFE`: Green LED ON, Buzzer OFF.
  * `WARNING` (Gas 30–60%): Yellow LED ON, 1Hz pulsing beep.
  * `DANGER` (Gas >60% OR Flame Detected): Red LED ON, continuous 2000Hz tone.
* **Silence Button**: Pressing GPIO8 (internal pull-up) mutes audio buzzer for 60 seconds without disabling visual warning LEDs.
</details>

<details>
<summary><b>🚗 Project 3: Ultrasonic Parking Assistant (Arduino Uno)</b> — <i>Click to expand</i></summary>

### Overview
Automotive proximity detection system utilizing an HC-SR04 ultrasonic sensor with a 5-sample acoustic noise filter and dynamic visual/audio feedback.

* **Physics Formula**:
  $$\text{Distance (cm)} = \frac{\text{pulseIn}(\text{ECHO}, \text{HIGH}) \times 0.034}{2}$$
* **Proximity Zones**:
  * **SAFE** (>60cm): Green LED, Silent.
  * **CAUTION** (30–60cm): Yellow LED, 800ms beep interval.
  * **CLOSE** (15–30cm): Yellow + Red LEDs, 300ms beep interval.
  * **DANGER** (<15cm): Red LED, continuous alarm tone.
</details>

<details>
<summary><b>🌱 Project 4: Smart Plant Watering Monitor (ESP32)</b> — <i>Click to expand</i></summary>

### Overview
Automated irrigation system regulating soil moisture and monitoring ambient temperature with non-chatter relay switching.

* **Hysteresis Relay Control**:
  ```text
  Soil Moisture < 30% ---> Relay ON (Pump Active)
  Soil Moisture > 40% ---> Relay OFF (Pump Idle)
  ```
* **Calibration Range**: `DRY_RAW = 2207` (open air), `WET_RAW = 656` (submerged tip).
* **Manual Override**: Pressing GPIO0 forces pump active for 5 seconds before returning to automatic tracking.
</details>

<details>
<summary><b>🌤️ Project 6: Wi-Fi Weather Dashboard (ESP32)</b> — <i>Click to expand</i></summary>

### Overview
Internet-connected weather station serving real-time sensor metrics locally on OLED and over HTTP/JSON web interfaces.

* **Web Endpoints**:
  * `http://<ESP32-IP>/` : Web UI with auto-refreshing (10s) card elements and dynamic background colors.
  * `http://<ESP32-IP>/data` : Structured JSON API:
    ```json
    {"temp": 28.5, "humidity": 65.0, "pressure": 1012.3, "altitude": 120.5, "light": 82}
    ```
* **Wi-Fi Health Check**: Auto-reconnect background loop evaluating Wi-Fi link status every 30 seconds.
</details>

<details>
<summary><b>⚙️ Project 9: Stepper Motor Precision Positioner (Arduino Uno)</b> — <i>Click to expand</i></summary>

### Overview
Angular position controller operating a 28BYJ-48 stepper motor via ULN2003 driver board with potentiometer target feedback and button homing.

* **Gear Ratio & Resolution**: 1:64 gearbox providing 2048 steps per full revolution ($\approx 0.176^\circ$ per step).
* **Pin Reassignment**: ULN2003 control shifted from D8–D11 to **IN1=D8, IN2=D9, IN3=D10, IN4=D7** to prevent signal conflicts with hardware SPI OLED (D11/D13).
* **Controls**: CW step (+45°), CCW step (-45°), Home reset (zero logical origin).
</details>

---

## ⚙️ Technical Innovations & Engineering Highlights

> [!IMPORTANT]
> **7-Pin SPI OLED Adaptation Across All Projects**  
> The original kit documentation specified a 4-pin I2C OLED display. However, the physical module provided in the hardware kit was a **7-Pin SPI SSD1306 OLED** (GND, VCC, D0, D1, RES, DC, CS). All projects (`P1`, `P2`, `P3`, `P4`, `P6`, `P9`, `P10`) were adapted to utilize hardware SPI constructors (`&SPI`) with optimized pin mappings.

### Hardware SPI OLED Pin Mapping Matrix

| Signal Line | Function | ESP32 GPIO | Arduino Uno Pin (P3 / P9) |
|---|---|:---:|:---:|
| **D0** | SPI Clock (SCK) | `GPIO18` | `D13` |
| **D1** | SPI Data (MOSI) | `GPIO23` | `D11` |
| **RES** | Hardware Reset | `GPIO16 / GPIO14 / GPIO19` | `D12 / A1` |
| **DC** | Data / Command Select | `GPIO17 / GPIO27 / GPIO21` | `D8 / D6` |
| **CS** | SPI Chip Select | `GPIO5` | `D7 / D5` |

### Key Code Optimizations
1. **Non-Blocking Execution**: Eliminated blocking `delay()` calls across all sketches in favor of state-machine timing loops powered by `millis()`.
2. **Acoustic Noise Filtering**: Implemented a 5-sample moving average filter in `P3` (Parking Sensor) to discard spurious ultrasonic echo reflections.
3. **Relay Chatter Prevention**: Implemented a 10% hysteresis window (30% to 40%) in `P4` (Plant Monitor) to protect relay contact coils from rapid toggling.
4. **Buzzer Transition Safety**: Resolved audio hang-ups in `P2` and `P3` by inserting explicit state transition checks before changing tone output frequencies.
5. **Zero-Latency Display Startup**: Updated setup routines in `P6`, `P9`, and `P10` to sample hardware sensors prior to rendering the initial OLED screen, eliminating blank or zeroed displays on boot.

---

## 🛠️ Hardware & Component Matrix

```text
home-assignments/
├── Microcontrollers
│   ├── ESP32 Dev Module (38-Pin Variant)
│   └── Arduino Uno R3 (ATmega328P)
├── Sensors & Transducers
│   ├── DHT22 / AM2302 (Temperature & Humidity)
│   ├── BMP280 (I2C Barometric Pressure & Altitude)
│   ├── MQ-2 (LPG / Smoke Flammable Gas Sensor)
│   ├── Infrared Flame Sensor (Digital Active-Low)
│   ├── HC-SR04 (Ultrasonic Rangefinder)
│   ├── PIR HC-SR501 (Pyroelectric Motion Sensor)
│   ├── LDR (Light Dependent Resistor in 10kΩ divider)
│   └── 10kΩ Potentiometers (Analog Target Control)
└── Actuators, Displays & Drivers
    ├── 0.96" 7-Pin SPI OLED Display (SSD1306 128x64)
    ├── 2-Channel Relay Module (Active-LOW 5V Coils)
    ├── 28BYJ-48 Stepper Motor + ULN2003 Driver
    ├── Active Buzzers (PWM & Direct Driven)
    └── Tactile Push Buttons & Tri-color Indicator LEDs
```

---

## 📂 Repository Layout

```text
home-assignments/
├── DOCUMENTATION.md                # Full technical report for all 7 projects
├── LICENSE                         # MIT License
├── README.md                       # Comprehensive repository submission index
│
├── p1-climate-monitor/             # P1: ESP32 Smart Room Climate Monitor
│   ├── README.md
│   └── p1_climate_monitor/
│       ├── demo_video/video_link.txt
│       └── p1_climate_monitor.ino
│
├── p2-gas-fire-alert/              # P2: ESP32 Gas & Fire Safety System
│   ├── README.md
│   └── p2_gas_fire_alert/
│       ├── demo_video/video_link.txt
│       └── p2_gas_fire_alert.ino
│
├── p3-parking-sensor/              # P3: Arduino Uno Ultrasonic Parking Assistant
│   ├── README.md
│   └── p3_parking_sensor/
│       ├── demo_video/video_link.txt
│       └── p3_parking_sensor.ino
│
├── p4-plant-monitor/               # P4: ESP32 Smart Plant Watering Monitor
│   ├── README.md
│   └── p4_plant_monitor/
│       ├── demo_video/video_link.txt
│       └── p4_plant_monitor.ino
│
├── p6-wifi-dashboard/              # P6: ESP32 Wi-Fi Weather Dashboard
│   ├── README.md
│   ├── config.h.example
│   └── p6_wifi_dashboard/
│       ├── config.h.example
│       ├── demo_video/video_link.txt
│       └── p6_wifi_dashboard.ino
│
├── p9-stepper-controller/          # P9: Arduino Uno Stepper Motor Positioner
│   ├── README.md
│   └── p9_stepper_controller/
│       ├── demo_video/video_link.txt
│       └── p9_stepper_controller.ino
│
└── p10-home-hub/                   # P10: Full IoT Home Automation Hub (Capstone)
    ├── README.md
    ├── config.h.example
    └── p10_home_hub/
        ├── config.h.example
        ├── demo_video/video_link.txt
        └── p10_home_hub.ino
```

---

## 🚀 Getting Started & Execution Guide

### 1. Clone Repository
```bash
git clone https://github.com/gupta-heli/home-assignments.git
cd home-assignments
```

### 2. Network Credentials Setup (P6 & P10)
For projects requiring Wi-Fi & MQTT connectivity:
1. Navigate to the project directory (e.g., `p10-home-hub/p10_home_hub/`).
2. Copy `config.h.example` to `config.h`:
   ```bash
   cp config.h.example config.h
   ```
3. Update `config.h` with your local Wi-Fi SSID, password, and MQTT username:
   ```cpp
   #define WIFI_SSID     "Your_WiFi_SSID"
   #define WIFI_PASSWORD "Your_WiFi_Password"
   #define MQTT_USERNAME "heligupta"
   ```

### 3. Arduino IDE Setup & Compilation
1. **Board Managers**:
   * ESP32: Add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   * Arduino Uno: Standard Arduino AVR Boards package.
2. **Required Libraries**:
   * `Adafruit SSD1306` & `Adafruit GFX Library`
   * `DHTesp` by beegee-tokyo
   * `Adafruit BMP280 Library` (for P6)
   * `ESPAsyncWebServer` & `AsyncTCP` (for P6 & P10 on ESP32)
   * `PubSubClient` by Nick O'Leary (for P10 MQTT)
3. **Upload**: Select target board and COM port, then click **Upload**. Open Serial Monitor at **115200 baud**.

---

## 📝 Submission Guidelines Compliance Checklist

- [x] **Dedicated Directory Structure**: All projects are isolated within clean subdirectories under `/home-assignments/`.
- [x] **Individual README Documentation**: Every project contains a detailed `README.md` with wiring summary, library versions, state tables, and step-by-step guides.
- [x] **Video Demonstration Links**: All projects include a `demo_video/video_link.txt` file referencing the uploaded demonstration recording on Google Drive.
- [x] **Descriptive Commit History**: Git history strictly adheres to standardized commit prefixes (`feat:`, `docs:`, `fix:`) with a minimum of 3 commits per project directory.
- [x] **Sensitive Credential Isolation**: Managed via `config.h.example` templates and tracked in `.gitignore` to prevent credential exposure.

---

## 📜 License

This repository is licensed under the [MIT License](LICENSE).

**Student Author**: Heli Gupta  
**Program**: IIT Jammu Summer School 2026 (IoT & Drones Program)  
**Repository**: [gupta-heli/home-assignments](https://github.com/gupta-heli/home-assignments)
