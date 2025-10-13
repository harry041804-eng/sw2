#include <Servo.h>
#include <math.h>

#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

unsigned long last_sampling_time;

float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  unsigned long dur = pulseIn(ECHO, HIGH, TIMEOUT);
  if (dur == 0) return NAN;
  return dur * SCALE;
}

uint8_t pwmFromDistance(float d) {
  if (isnan(d)) return 255;
  if (d < _DIST_MIN) d = _DIST_MIN;
  if (d > _DIST_MAX) d = _DIST_MAX;
  float delta = fabsf(d - 200.0f);
  float pwmf = (delta * 255.0f) / 100.0f;
  if (pwmf < 0.0f) pwmf = 0.0f;
  if (pwmf > 255.0f) pwmf = 255.0f;
  return (uint8_t)(pwmf + 0.5f);
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  Serial.begin(57600);
  last_sampling_time = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - last_sampling_time < INTERVAL) return;

  float distance = USS_measure(PIN_TRIG, PIN_ECHO);
  uint8_t pwm = pwmFromDistance(distance);
  analogWrite(PIN_LED, pwm);

  Serial.print("Min:");       Serial.print(_DIST_MIN);
  Serial.print(",distance:"); Serial.print(distance);
  Serial.print(",pwm:");      Serial.print((int)pwm);
  Serial.print(",Max:");      Serial.print(_DIST_MAX);
  Serial.println();

  last_sampling_time = now;
}
