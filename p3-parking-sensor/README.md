# Project 3: Ultrasonic Parking Assistant

A parking assistant device similar to those found in modern cars. It measures distance using an ultrasonic sensor and provides dynamic feedback through LED color changes and a buzzer with increasing beep frequencies as an object gets closer. It also displays the exact distance in cm on an OLED screen with a visual proximity bar graph.

## Components Used
- **Arduino Uno R3**
- **HC-SR04 Ultrasonic Sensor**
- **Active Buzzer**
- **LEDs**: Red, Yellow, Green
- **Resistors**: 220Ω (×3 for LEDs)
- **OLED Display 0.96"** — 7-pin SPI SSD1306 module used instead of the kit's I2C version
- **Breadboard & Jumper Wires**

## ⚠️ Hardware Notes
- **OLED**: The kit sheet lists an I2C OLED (4-pin), but the supplied module is the **7-pin SPI SSD1306**. This project wires it using hardware SPI (SCK=D13, MOSI=D11 on Uno). If using an I2C module, adjust the code and wire SDA to A4 and SCL to A5.

## Wiring Summary / Wiring Diagram

```text
[HC-SR04 Ultrasonic Sensor]
  VCC  --------------> 5V
  GND  --------------> GND
  TRIG --------------> Analog Pin A2
  ECHO --------------> Analog Pin A3

[OLED Display (SPI SSD1306 7-Pin)]
  VCC  --------------> 5V
  GND  --------------> GND
  D0 (SCK/CLK) ------> Digital Pin D13 (Hardware SPI)
  D1 (MOSI/DATA) ----> Digital Pin D11 (Hardware SPI)
  RES (Reset) -------> Digital Pin D12
  DC (Data/Command) -> Digital Pin D8
  CS (Chip Select) --> Digital Pin D7

[LED Indicators]
  Green LED (Safe) ----> Pin D2 ---> 220Ω resistor ---> Green LED Anode ---> GND
  Yellow LED (Warn) ---> Pin D3 ---> 220Ω resistor ---> Yellow LED Anode --> GND
  Red LED (Danger) ----> Pin D4 ---> 220Ω resistor ---> Red LED Anode -----> GND

[Buzzer]
  Buzzer Positive -----> Pin D6 (PWM capable) ---> GND
```

## Libraries
- `Adafruit_SSD1306`
- `Adafruit_GFX`

## How to Run
1. Wire components according to the wiring diagram.
2. Connect the Arduino Uno R3 via USB.
3. Open `p3_parking_sensor/p3_parking_sensor.ino` in the Arduino IDE.
4. Select Board: **Arduino Uno** and your COM port.
5. Upload the sketch and open the Serial Monitor set to **115200 baud rate**.

## Expected Output
- **OLED Display**: Shows live distance in cm (large text) and a proximity bar graph that fills as objects get closer.
- **Alert Zones**:
  - **SAFE (>60cm)**: Green LED ON, Buzzer OFF.
  - **CAUTION (30–60cm)**: Yellow LED ON, Buzzer beeps slowly (every 800ms).
  - **CLOSE (15–30cm)**: Yellow + Red LEDs ON, Buzzer beeps fast (every 300ms).
  - **DANGER (<15cm)**: Red LED ON, Buzzer provides a continuous alarm.
- **Serial Monitor**: Logs the distance and active zone every 500ms.

## Physics Formula Note
Distance calculation uses the formula: `distance = pulseIn(ECHO, HIGH) * 0.034 / 2`
- The sensor emits a 40kHz ultrasonic burst.
- `ECHO` stays `HIGH` for the sound wave's round-trip time in microseconds.
- Speed of sound in air is approximately 0.034 cm/µs.
- The result is divided by 2 to account for the sound traveling to the object and reflecting back.

## Demo Video
- [Demo Video Google Drive Link](https://drive.google.com/file/d/1BDpT4dZ_QZiptEosnnRhm9D1HPPy0j5Y/view?usp=drivesdk)
