# Project 9: Stepper Motor Precision Positioner

A precision positioning system using an Arduino Uno R3, 28BYJ-48 Stepper Motor with ULN2003 Driver, a potentiometer for target setting, push buttons for step control/homing, and a 0.96" SPI OLED display.

## Components Used
- **Arduino Uno R3**
- **28BYJ-48 Stepper Motor + ULN2003 Driver Board**
- **10kΩ Potentiometer**
- **Push Buttons ×3** (CW 45° step, CCW 45° step, Home reset)
- **OLED Display 0.96"** — 7-pin SPI SSD1306 module (adapted from kit sheet I2C spec)
- **Breadboard & Jumper Wires**

## ⚠️ Hardware Note & Pin Reassignment
- **OLED**: The kit's OLED is the 7-pin SPI SSD1306 module, which uses Arduino Uno hardware SPI pins (**SCK=D13, MOSI=D11**).
- **Pin Shift**: To keep hardware SPI free of signal conflicts, the ULN2003 stepper driver control pins were shifted from D8-D11 to **IN1=D8, IN2=D9, IN3=D10, IN4=D7**.

## Wiring Summary / Wiring Diagram

```text
[ULN2003 Stepper Driver Module]
  VCC  --------------> 5V
  GND  --------------> GND
  IN1  --------------> Digital Pin D8
  IN2  --------------> Digital Pin D9
  IN3  --------------> Digital Pin D10
  IN4  --------------> Digital Pin D7

[10kΩ Potentiometer]
  Outer Pin 1 -------> 5V
  Outer Pin 2 -------> GND
  Wiper (Center) ----> Analog Pin A0

[Push Buttons]
  CW Button (45° CW)  --> Pin D2 (INPUT_PULLUP) ---> GND
  CCW Button (45° CCW) -> Pin D3 (INPUT_PULLUP) ---> GND
  Home Button (Zero)  --> Pin D4 (INPUT_PULLUP) ---> GND

[OLED Display (SPI SSD1306 7-Pin)]
  VCC  --------------> 5V / 3.3V
  GND  --------------> GND
  D0 (SCK/CLK) ------> Digital Pin D13 (Hardware SPI Clock)
  D1 (MOSI/DATA) ----> Digital Pin D11 (Hardware SPI Data)
  RES (Reset) -------> Analog Pin A1
  DC (Data/Command) -> Digital Pin D6
  CS (Chip Select) --> Digital Pin D5
```

## Libraries
- `Stepper` (Arduino built-in)
- `Adafruit_SSD1306`
- `Adafruit_GFX`

## How to Run
1. Wire per the table above (note **IN4=D7**).
2. Connect Arduino Uno R3 via USB.
3. Open `p9_stepper_controller/p9_stepper_controller.ino` in Arduino IDE.
4. Select Board: **Arduino Uno** and your COM port.
5. Upload the sketch and open Serial Monitor set to **115200 baud**.

## Expected Output
- **Potentiometer**: Adjusting the knob dynamically updates the target angle (0° to 360°) displayed on the OLED screen.
- **Buttons**:
  - **CW Button**: Advances motor by +45° (512 steps).
  - **CCW Button**: Reverses motor by -45° (-512 steps).
  - **Home Button**: Zeroes logical position (`currentSteps = 0`) without moving motor.
- **OLED Screen**: Displays live status:
  - `Current`: Current logical angle (0.0°–360.0°)
  - `Target`: Target angle set by potentiometer
  - `Dir`: Active direction (`CW`, `CCW`, `HOME SET`)
  - `Steps`: Cumulative step count

## 28BYJ-48 Motor Notes
- Has a gear reduction ratio of 1:64.
- Effective 2048 steps = 1 full output-shaft revolution.
- Speed is set to 10 RPM (do not exceed ~15 RPM to avoid skipping steps under load).

## Demo Video
- [Demo Video Google Drive Link](https://drive.google.com/file/d/1s1UZSu1eKSoa8EFJN89dSSXOmbYgaI4E/view?usp=drivesdk)
