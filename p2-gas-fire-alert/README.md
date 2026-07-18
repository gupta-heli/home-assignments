# Project 2: Gas & Fire Safety Alert System

A dual-sensor safety alert device using Arduino Uno R3 that continuously monitors LPG/smoke gas levels via an MQ-2 sensor and detects flames using an active-low infrared flame sensor. The system provides visual LED indicators, multi-pattern buzzer alerts (slow beep for WARNING, continuous alarm for DANGER), and a physical 30-second silence override button.

## Components Used
- **Arduino Uno R3**
- **MQ-2 Gas Sensor** (LPG / Smoke)
- **Infrared Flame Sensor** (Digital + Analog output)
- **Active Buzzer**
- **LEDs**: Red (Danger), Yellow (Warning), Green (Safe)
- **Resistors**: 220Ω (×3 for LEDs)
- **Push Button**: Silence override button (using internal `INPUT_PULLUP`)
- **Breadboard & Jumper Wires**

## ⚠️ Sensor & Hardware Notes
- **MQ-2 Gas Sensor Warm-up**: The MQ-2 requires a 5V supply (3.3V is insufficient) and takes approximately 1 to 2 minutes after powering on for its internal heater coil to stabilize and provide accurate analog readings.
- **Active-Low Flame Sensor**: The digital output (`DO`) of the flame sensor module is **active-LOW** (`LOW` = flame/IR light detected). The internal phototransistor pulls the signal line down to GND when infrared light at the threshold is present.

## Wiring Summary / Wiring Diagram

```text
[MQ-2 Gas Sensor]
  VCC  --------------> 5V (Requires 5V, 3.3V will not heat element)
  GND  --------------> GND
  AOUT --------------> Analog Pin A0

[Flame Sensor]
  VCC  --------------> 5V
  GND  --------------> GND
  DO   --------------> Digital Pin D7 (Digital active-LOW output)
  AO   --------------> Analog Pin A1 (Optional analog reading)

[LED Indicators]
  Green LED (Safe) ----> Pin D10 ---> 220Ω resistor ---> Green LED Anode ---> GND
  Yellow LED (Warn) ---> Pin D11 ---> 220Ω resistor ---> Yellow LED Anode --> GND
  Red LED (Danger) ----> Pin D12 ---> 220Ω resistor ---> Red LED Anode -----> GND

[Buzzer & Controls]
  Buzzer Positive -----> Pin D9 (PWM capable for tone generation) ---> GND
  Silence Button -------> Pin D8 (INPUT_PULLUP) ---> GND
```

## Libraries
- Standard Arduino Core libraries (`tone()`, `noTone()`, `digitalRead()`, `analogRead()`).

## How to Run
1. Wire components per the wiring table above.
2. Connect Arduino Uno R3 via USB to your computer.
3. Open `p2_gas_fire_alert/p2_gas_fire_alert.ino` in the Arduino IDE.
4. Select Board: **Arduino Uno** and select the appropriate COM Port.
5. Upload the sketch and open Serial Monitor set to **115200 baud rate**.
6. Allow **1–2 minutes** for the MQ-2 sensor heater to warm up.

## Expected Output
- **Serial Monitor**: Prints formatted status logs every second:
  `GAS: 25% | FLAME: NONE | STATUS: SAFE`
  `GAS: 45% | FLAME: NONE | STATUS: WARNING`
  `GAS: 70% | FLAME: DETECTED | STATUS: DANGER`
- **Alert States**:
  - **SAFE** (Gas < 30% and No Flame): Green LED ON, Buzzer OFF.
  - **WARNING** (Gas 30%–60% and No Flame): Yellow LED ON, Buzzer beeps 1 pulse/sec (150ms tone every 1000ms).
  - **DANGER** (Gas > 60% OR Flame Detected): Red LED ON, Buzzer continuous alarm tone (2000 Hz).
- **Independent Alarm Logic**: Gas danger triggers standard high alarm; flame detection immediately triggers DANGER state even if gas level is low.

## Bonus Challenge Implemented
Silence push button connected to Pin D8 (`INPUT_PULLUP`). Pressing the button during WARNING or DANGER state mutes the buzzer for 30 seconds while keeping LEDs active. If the hazardous condition persists after 30 seconds, the buzzer automatically reactivates.

## Demo Video
- [Demo Video Google Drive Link](https://drive.google.com/file/d/1r8D-ZYj0fd1-MBPzozhY0dkahqsEN-Ib/view?usp=drivesdk)
