// C++ code
//

#include <Servo.h>

// pin setup
Servo smotor;
int pwd_port = 3;
int rled = 4;
int gled = 5;
int bled = 6;
int button = 8;
int gas_signal = A5;
int angle = 0;
// state variables
bool is_critical = false;
int threshold = 370;
int threshold_C = 700;
int iddle_delay = 3000;
int cstate = 0;
int iddle = 0;
int danger = 1;
int critical = 2;
int manual = 3;
// helpers
int led_timer = 0;
bool ledState = LOW;

int d_read = 0;
bool can_read = true;

void setup() {
  Serial.begin(9600);
  // leds
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  // button
  pinMode(button, INPUT_PULLUP);
  // serbo mottor init
  smotor.attach(pwd_port);
  smotor.write(angle);
}

void led_pattern(int ledPin) {

  if (is_critical) {

    if (millis() - led_timer >= 200) {
      led_timer = millis();

      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
  }
}

void led_states(int r, int g, int b) {
  digitalWrite(rled, r);
  digitalWrite(gled, g);
  digitalWrite(bled, b);
}

void loop() {

  int ppm = analogRead(gas_signal);
  bool is_pressed = digitalRead(button) == LOW;

  if (cstate == danger && millis() - d_read < iddle_delay) {

    cstate = danger;
    can_read = false;

  } else {

    cstate = iddle;
    is_critical = false;
    can_read = true;
  }

  if (is_pressed) {
    cstate = manual;

  } else {
    if (ppm > threshold) {
      cstate = danger;

      if (can_read) {
        d_read = millis();
        can_read = false;
      }
    }

    if (ppm > threshold_C) {

      cstate = critical;
      is_critical = true;
    }
  }

  Serial.println(ppm);

  switch (cstate) {

  case 0:
    led_states(LOW, HIGH, LOW);

    smotor.write(0);
    Serial.println("Modo IDLE - Ambiente Seguro");

    break;
  case 1:
    led_states(HIGH, LOW, LOW);
    smotor.write(45);
    Serial.println("Ventilacion activada por detección de gas");
    // delay(iddle_delay);
    break;
  case 2:
    led_states(LOW, LOW, LOW);
    led_pattern(rled);
    smotor.write(90);
    Serial.println("ALERTA CRITICA: Niveles peligrosos de gas");
    break;

  case 3:
    led_states(LOW, LOW, HIGH);
    smotor.write(90);
    Serial.println("Modo manual activado");
    break;
  }

  delay(10);
}