#include <Servo.h>

const int PIN_TRIG = 2;
const int PIN_ECHO = 3;
const int PIN_SERVO = 6;
const int PIN_LED = 9;

const float ALPHA = 0.3;

Servo sv;
float emaDist = 30.0;

float measureCM() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (duration == 0) return 400.0;
  return duration * 0.0343 / 2.0;
}

int mapProportional(float d) {
  float cd = d;
  if (cd < 18.0) cd = 18.0;
  if (cd > 36.0) cd = 36.0;
  return (int)((cd - 18.0) * (180.0 / 18.0) + 0.5);
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED, OUTPUT);
  sv.attach(PIN_SERVO);
}

void loop() {
  float d = measureCM();
  emaDist = ALPHA * d + (1.0 - ALPHA) * emaDist;

  int angle;
  if (emaDist <= 18.0) {
    angle = 0;
    digitalWrite(PIN_LED, LOW);
  } else if (emaDist >= 36.0) {
    angle = 180;
    digitalWrite(PIN_LED, LOW);
  } else {
    angle = mapProportional(emaDist);
    digitalWrite(PIN_LED, HIGH);
  }

  sv.write(angle);
  delay(30);
}
