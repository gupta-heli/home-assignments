# IoT Assignment 3 — Comprehensive Project Documentation

**Student Name**: Heli Gupta  
**Repository Link**: [https://github.com/gupta-heli/home-assignments.git](https://github.com/gupta-heli/home-assignments.git)

---

## 📌 Submission Overview

- **Student Name**: Heli Gupta
- **Course Assignment**: IOT Assignment 3
- **Repository Link**: [https://github.com/gupta-heli/home-assignments.git](https://github.com/gupta-heli/home-assignments.git)
- **Primary Microcontrollers Used**: Arduino Uno R3 & ESP32 Dev Module
- **Submission Guidelines Compliance**:
  - Each project is isolated within its dedicated directory (`p1-climate-monitor`, `p2-gas-fire-alert`, `p3-parking-sensor`, `p4-plant-monitor`, `p6-wifi-dashboard`, `p9-stepper-controller`, `p10-home-hub`).
  - Followed strict 3-commit policy per project (`docs:`, `feat:`, `fix:`).
  - All hardware adaptations (7-pin SPI OLED SSD1306, DHT22) documented with updated pinout tables.
  - Sensitive network credentials excluded using `config.h` & `.gitignore`.

---

## 📁 Projects Summary & Status Table

| Project | Name | Student | Controller | Key Components | Demo Video Link | Status |
|---|---|---|---|---|---|---|
| **P1** | Smart Room Climate Monitor | Heli Gupta | ESP32 | DHT22, SPI OLED, Buzzer, LEDs | [Google Drive Link](https://drive.google.com/file/d/1vLFONvlPQga3nKYxwgaMuzZK6_eY-hea/view?usp=drivesdk) | ✅ Pushed |
| **P2** | Dual-Sensor Gas & Fire Safety Alert System | Heli Gupta | ESP32 | MQ-2, Flame Sensor, SPI OLED, Buzzer, LEDs, Mute Btn | [Google Drive Link](https://drive.google.com/file/d/1r8D-ZYj0fd1-MBPzozhY0dkahqsEN-Ib/view?usp=drivesdk) | ✅ Pushed |
| **P3** | Ultrasonic Parking Assistant | Heli Gupta | Arduino Uno | HC-SR04, Active Buzzer, LEDs, SPI OLED | [Google Drive Link](https://drive.google.com/file/d/1BDpT4dZ_QZiptEosnnRhm9D1HPPy0j5Y/view?usp=drivesdk) | ✅ Pushed |
| **P4** | Smart Plant Watering Monitor | Heli Gupta | ESP32 | Soil Moisture Sensor, DHT22, 2-Ch Relay, SPI OLED, Button | [Google Drive Link](https://drive.google.com/file/d/1DveY83dGN6ks4dDuBE6NxnJa8Oq3XJ8J/view?usp=drivesdk) | ✅ Pushed |
| **P6** | Wi-Fi Weather Dashboard | Heli Gupta | ESP32 | DHT22, BMP280 (I2C), LDR, SPI OLED, AsyncWebServer | [Google Drive Link](https://drive.google.com/file/d/15fWOxQ6lwCGY5wamdTr4Z5B-RtmRquOx/view?usp=drivesdk) | ✅ Pushed |
| **P9** | Stepper Motor Precision Positioner | Heli Gupta | Arduino Uno | 28BYJ-48 + ULN2003, Potentiometer, Buttons ×3, SPI OLED | [Google Drive Link](https://drive.google.com/file/d/1s1UZSu1eKSoa8EFJN89dSSXOmbYgaI4E/view?usp=drivesdk) | ✅ Pushed |
| **P10** | Full IoT Home Automation Hub (Capstone) | Heli Gupta | ESP32 | DHT22, MQ-2, PIR, LDR, 2-Ch Relay, SPI OLED, AsyncWebServer, MQTT | [Google Drive Link](https://drive.google.com/file/d/1ZG5S2_FP5dle4uLXW0F3x9J1LTlG_Mzw/view?usp=drivesdk) | ✅ Pushed |

---

## 🛠️ Project 1: Smart Room Climate Monitor

- **Author**: Heli Gupta
- **Repository Path**: `/home-assignments/p1-climate-monitor/`

### 1. Description
Real-time indoor climate monitoring system built on ESP32 that continuously measures temperature and humidity, logs CSV-formatted data over Serial, displays readings on a 7-pin SPI OLED, and triggers high-temperature alerts.

### 2. Components Used
- ESP32 Dev Board
- DHT22 Sensor (AM2302)
- 0.96" SPI OLED Display (SSD1306)
- Active Buzzer
- Red LED & Green LED + 220Ω Resistors

### 3. Wiring Reference
| Component | Pin | ESP32 GPIO |
|---|---|---|
| DHT22 DATA | Data | GPIO4 (10kΩ pull-up) |
| Active Buzzer | Positive | GPIO14 |
| Red LED (Alert) | Anode | GPIO25 → 220Ω → GND |
| Green LED (Normal)| Anode | GPIO26 → 220Ω → GND |
| OLED D0 (SCK) | Clock | GPIO18 (SPI) |
| OLED D1 (MOSI) | Data | GPIO23 (SPI) |
| OLED RES | Reset | GPIO16 |
| OLED DC | Data/Cmd | GPIO17 |
| OLED CS | Chip Sel | GPIO5 |

### 4. Key Logic & Features
- **Alert Threshold**: Triggers continuous buzzer tone and Red LED when temperature exceeds 38°C.
- **Daily Tracker**: Tracks maximum and minimum temperature/humidity values over time.
- **Display Output**: Page cycling between live readings and daily high/low statistics.

---

## 🛠️ Project 2: Dual-Sensor Gas & Fire Safety Alert System

- **Author**: Heli Gupta
- **Repository Path**: `/home-assignments/p2-gas-fire-alert/`

### 1. Description
Safety system combining flammable gas detection (MQ-2) and flame detection. Features multi-level visual/audible alarms, OLED state reporting, and a 60-second non-blocking alarm silence push button.

### 2. Components Used
- ESP32 Dev Board
- MQ-2 Flammable Gas Sensor
- Flame Sensor Module (Digital)
- 0.96" SPI OLED (SSD1306)
- Active Buzzer, Red & Green LEDs
- Push Button (Silence Alarm)

### 3. Wiring Reference
| Component | Pin | ESP32 GPIO |
|---|---|---|
| MQ-2 AOUT | Analog | GPIO34 |
| Flame Sensor OUT | Digital | GPIO35 |
| Silence Button | Leg | GPIO0 (INPUT_PULLUP) |
| Buzzer | Positive | GPIO14 |
| Red LED (Danger) | Anode | GPIO25 → 220Ω → GND |
| Green LED (Normal)| Anode | GPIO26 → 220Ω → GND |
| OLED Pins | SPI | SCK:18, MOSI:23, CS:5, DC:17, RES:16 |

### 4. Safety Logic & Thresholds
- **DANGER State**: MQ-2 gas reading >40% or Flame Sensor == LOW → Red LED flashes, active buzzer pulses.
- **Mute Timer**: Pressing silence button suppresses audio buzzer for 60 seconds without affecting visual LED warnings.

---

## 🛠️ Project 3: Ultrasonic Parking Assistant

- **Author**: Heli Gupta
- **Repository Path**: `/home-assignments/p3-parking-sensor/`

### 1. Description
Automated car parking proximity sensor built on Arduino Uno R3. Uses an HC-SR04 ultrasonic sensor to calculate exact target distance and provides dynamic audio-visual feedback across 4 distinct zones.

### 2. Components Used
- Arduino Uno R3
- HC-SR04 Ultrasonic Sensor
- Active Buzzer (PWM)
- Red, Yellow, Green LEDs + 220Ω Resistors
- 0.96" SPI OLED (SSD1306)

### 3. Physics & Distance Formula
$$\text{Distance (cm)} = \frac{\text{pulseIn}(\text{ECHO}, \text{HIGH}) \times 0.034}{2}$$
- Speed of sound in air $\approx 0.034 \text{ cm/\mu s}$. Division by 2 compensates for ultrasonic wave round-trip travel time.
- Implements **5-sample averaging** to filter out acoustic reflections and noise.

### 4. Proximity Zones & Logic Table
| Zone | Distance | LEDs Active | Buzzer Behavior |
|---|---|---|---|
| **SAFE** | >60 cm | Green ON | Silent |
| **CAUTION** | 30–60 cm | Yellow ON | Beep every 800 ms |
| **CLOSE** | 15–30 cm | Yellow + Red ON | Beep every 300 ms |
| **DANGER** | <15 cm | Red ON | Continuous Alarm Tone |

---

## 🛠️ Project 4: Smart Plant Watering Monitor

- **Author**: Heli Gupta
- **Repository Path**: `/home-assignments/p4-plant-monitor/`

### 1. Description
Automated irrigation system with ESP32. Monitors soil moisture level and ambient temperature, automatically toggles a water pump relay via hysteresis logic, and provides manual button override.

### 2. Components Used
- ESP32 Dev Board
- Capacitive Soil Moisture Sensor (3.3V compatible)
- DHT22 Temperature Sensor
- 2-Channel Relay Module (Active-LOW)
- 0.96" SPI OLED Display

### 3. Hysteresis Pump Logic
```text
          [Soil Moisture < 30%] ---> PUMP ON
          [Soil Moisture > 40%] ---> PUMP OFF
```
*Hysteresis gap (30% to 40%) prevents rapid relay chatter.*

### 4. Sensor Calibration
- `DRY_RAW` = 2207 (Sensor in dry open air)
- `WET_RAW` = 656 (Sensor submersed in water)

---

## 🛠️ Project 6: Wi-Fi Weather Dashboard

- **Author**: Heli Gupta
- **Repository Path**: `/home-assignments/p6-wifi-dashboard/`

### 1. Description
IoT weather station hosting an asynchronous web server. Reads temperature, humidity, pressure, altitude, and ambient light, displaying live data on both an SPI OLED screen and an HTML web dashboard.

### 2. Components Used
- ESP32 Dev Board
- BMP280 Barometric Pressure Sensor (I2C)
- DHT22 Sensor
- LDR Light Sensor (Voltage Divider)
- 0.96" SPI OLED (SSD1306)

### 3. Web Endpoints & Features
- `http://<ESP32-IP>/` : Web Dashboard with auto-refresh (10s) & dynamic temperature background color.
- `http://<ESP32-IP>/data` : Returns structured JSON data (`temp`, `humidity`, `pressure`, `altitude`, `light`).
- **Auto-Reconnection**: Background Wi-Fi check every 30 seconds.

---

## 🛠️ Project 9: Stepper Motor Precision Positioner

- **Author**: Heli Gupta
- **Repository Path**: `/home-assignments/p9-stepper-controller/`

### 1. Description
Precision angular position control system using Arduino Uno R3, ULN2003 stepper driver, 28BYJ-48 stepper motor, 10kΩ potentiometer for target input, and push buttons for direction step & homing.

### 2. Gear Reduction & Resolution
- **Gear Ratio**: 1:64 reduction gearbox.
- **Resolution**: 2048 effective steps per output shaft revolution ($\approx 0.176^\circ$ per step).

### 3. Pin Shift Explanation
- Arduino Uno hardware SPI requires **D11 (MOSI)** and **D13 (SCK)** for the OLED.
- Stepper driver pins were shifted from D8–D11 to **IN1=D8, IN2=D9, IN3=D10, IN4=D7** to eliminate signal pin conflicts.

---

## 🛠️ Project 10: Full IoT Home Automation Hub (Capstone)

- **Author**: Heli Gupta
- **Repository Path**: `/home-assignments/p10-home-hub/`

### 1. Description
Complete smart home automation hub integrating DHT22, MQ-2, PIR, LDR, 2-Channel Relay, SPI OLED, AsyncWebServer, and MQTT cloud publishing (`broker.hivemq.com` on topic `iitjammu/heligupta/home`).

### 2. System Architecture Diagram
```text
[DHT22] [MQ-2] [PIR] [LDR] ----> readSensors()
                                     |
                                     v
                          applyAutomationRules()
                      (Safety > Fan Rule > Light Rule)
                                     |
                +--------------------+--------------------+
                |                    |                    |
          [Relay: Fan]         [Relay: Light]       [Buzzer/RedLED]
                |                    |                    |
                +--------------------+--------------------+
                                     |
            +------------------------+------------------------+
            |                                                 |
  AsyncWebServer (Port 80)                        MQTT Publishing (30s)
  Dashboard, /data JSON,                          broker.hivemq.com
  /fan/toggle, /light/toggle                      topic: iitjammu/heligupta/home
```

### 3. Automation Rules & Priority Matrix
1. **Safety Emergency (Highest Priority)**: If MQ-2 Gas >60% $\rightarrow$ Buzzer ON, Red LED ON, **force OFF both relays** (overrides manual toggles).
2. **Fan Control**: Auto ON if Temp >33°C, Auto OFF if Temp <28°C. Manual override duration: 10 minutes.
3. **Smart Lighting**: Auto ON if LDR <20% AND PIR Motion detected. Auto OFF if LDR >60% or no motion for 3 minutes.
