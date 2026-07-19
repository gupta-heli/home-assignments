/*
  Project 2: Gas & Fire Safety Alert System
  Board: Arduino Uno R3
  Sensors: MQ-2 Gas Sensor, Active-Low IR Flame Sensor
*/

#define MQ2_PIN      A0
#define FLAME_DO_PIN 7
#define FLAME_AO_PIN A1
#define GREEN_LED    10
#define YELLOW_LED   11
#define RED_LED      12
#define BUZZER_PIN   9
#define SILENCE_BTN  8   // bonus: silence button

unsigned long lastBeep = 0;
unsigned long lastLog = 0;
unsigned long silenceUntil = 0;
bool muted = false;

enum State { SAFE, WARNING, DANGER };
State currentState = SAFE;

void setup() {
  Serial.begin(115200);
  pinMode(FLAME_DO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SILENCE_BTN, INPUT_PULLUP);
  Serial.println("MQ-2 warming up (allow ~2 min for stable readings)...");
}

void setLEDs(State s) {
  digitalWrite(GREEN_LED, s == SAFE ? HIGH : LOW);
  digitalWrite(YELLOW_LED, s == WARNING ? HIGH : LOW);
  digitalWrite(RED_LED, s == DANGER ? HIGH : LOW);
}

void handleBuzzer(State s) {
  if (muted && millis() < silenceUntil) {
    noTone(BUZZER_PIN);
    return;
  }
  muted = false;

  static State lastState = SAFE;
  if (s != lastState) {
    noTone(BUZZER_PIN); // Stop any playing tone on state transition
    lastState = s;
    if (s == WARNING) {
      lastBeep = 0; // Force immediate beep on warning transition
    }
  }

  if (s == SAFE) {
    noTone(BUZZER_PIN);
  } else if (s == WARNING) {
    // 1 beep/sec
    if (millis() - lastBeep > 1000) {
      lastBeep = millis();
      tone(BUZZER_PIN, 1500, 150);
    }
  } else { // DANGER - continuous alarm
    tone(BUZZER_PIN, 2000); // continuous tone, stays on until state changes
  }
}

void loop() {
  int gasRaw = analogRead(MQ2_PIN);
  int gasPercent = map(gasRaw, 0, 1023, 0, 100);
  bool flameDetected = (digitalRead(FLAME_DO_PIN) == LOW); // active-low sensor

  if (digitalRead(SILENCE_BTN) == LOW && currentState != SAFE) {
    muted = true;
    silenceUntil = millis() + 30000;
  }

  if (gasPercent > 60 || flameDetected) {
    currentState = DANGER;
  } else if (gasPercent > 30) {
    currentState = WARNING;
  } else {
    currentState = SAFE;
  }

  setLEDs(currentState);
  handleBuzzer(currentState);

  if (millis() - lastLog > 1000) {
    lastLog = millis();
    Serial.print("GAS: "); Serial.print(gasPercent); Serial.print("% | ");
    Serial.print("FLAME: "); Serial.print(flameDetected ? "DETECTED" : "NONE"); Serial.print(" | ");
    Serial.print("STATUS: ");
    Serial.println(currentState == SAFE ? "SAFE" : currentState == WARNING ? "WARNING" : "DANGER");
  }
}
