/*
  Project 2: Gas & Fire Safety Alert System
  Board: Arduino Uno R3
  Sensors: MQ-2 Gas Sensor, Active-Low IR Flame Sensor
*/

#define MQ2_PIN      A0
#define FLAME_DO_PIN 7
#define GREEN_LED    10
#define YELLOW_LED   11
#define RED_LED      12
#define BUZZER_PIN   9

enum State { SAFE, WARNING, DANGER };
State currentState = SAFE;

unsigned long lastLog = 0;

void setup() {
  Serial.begin(115200);
  pinMode(FLAME_DO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("MQ-2 warming up (allow ~2 min for stable readings)...");
}

void setLEDs(State s) {
  digitalWrite(GREEN_LED, s == SAFE ? HIGH : LOW);
  digitalWrite(YELLOW_LED, s == WARNING ? HIGH : LOW);
  digitalWrite(RED_LED, s == DANGER ? HIGH : LOW);
}

void loop() {
  int gasRaw = analogRead(MQ2_PIN);
  int gasPercent = map(gasRaw, 0, 1023, 0, 100);
  bool flameDetected = (digitalRead(FLAME_DO_PIN) == LOW); // active-low sensor

  if (gasPercent > 60 || flameDetected) {
    currentState = DANGER;
  } else if (gasPercent > 30) {
    currentState = WARNING;
  } else {
    currentState = SAFE;
  }

  setLEDs(currentState);

  if (currentState == SAFE) {
    digitalWrite(BUZZER_PIN, LOW);
  } else if (currentState == WARNING) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, HIGH);
  }

  if (millis() - lastLog > 1000) {
    lastLog = millis();
    Serial.print("GAS: "); Serial.print(gasPercent); Serial.print("% | ");
    Serial.print("FLAME: "); Serial.print(flameDetected ? "DETECTED" : "NONE"); Serial.print(" | ");
    Serial.print("STATUS: ");
    Serial.println(currentState == SAFE ? "SAFE" : currentState == WARNING ? "WARNING" : "DANGER");
  }
}
