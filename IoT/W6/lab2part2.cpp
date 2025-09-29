#include <Adafruit_LiquidCrystal.h>
#include <Keypad.h>

// LCD
Adafruit_LiquidCrystal lcd_1(0);
unsigned long lcdScrollTimer = 0;
int lcdScrollDelay = 300;
bool scrolling = false;

// Leds y buzzer
int red_led = 3;
int green_led = 2;
int alarm = 13;

// Variables Leds Timer
unsigned long ledTimer = 0;
bool ledState = false;

// Variables Buzzer Timer
unsigned long buzzerTimer = 0;
bool buzzerOn = false;
bool Error_sensor_p = false;

// Ultrasonido
const int Trigger = 5;
const int Echo = 4;

// Teclado
const byte filas = 4;
const byte columnas = 3;
byte pinesFilas[] = {6, 7, 8, 9};
byte pinesColumnas[] = {10, 11, 12};
char teclas[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
Keypad keybo = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

// Password
String password = "1234";
String prov = "";
int current_state = 0;
int try_cnt = 0;
bool is_correct = true;

// ----------- SETUP ------------
void setup() {
  Serial.begin(9600);

  // Alarm & leds
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(alarm, OUTPUT);

  // Ultrasonido
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  digitalWrite(Trigger, LOW);

  // LCD
  lcd_1.begin(16, 2);
  lcd_1.print("Sistema Listo");
  delay(2000);
  lcd_1.clear();
}

// ----------- FUNCIONES ----------
bool is_close() {
  long t, d;
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);
  t = pulseIn(Echo, HIGH);
  d = t / 59;

  if (d >= 300 || d == 0) {
    Error_sensor_p = true;
    return false;
  }
  Error_sensor_p = false;
  return d < 20;
}

void led_patter(int id, int del) {
  if (is_correct) {
    if (millis() - ledTimer >= del) {
      ledState = !ledState;
      digitalWrite(id, ledState);
      ledTimer = millis();
    }
  } else {
    digitalWrite(id, HIGH);
  }
}

void update_string(char pressed_key) {
  if (pressed_key) {
    prov += pressed_key;
    current_state++;
    lcd_1.clear();
    lcd_1.println("STRING: ");
    lcd_1.println(prov);
  }
}

void reset_states(int state, String cad, int cnt, bool correct) {
  current_state = state;
  prov = cad;
  try_cnt = cnt;
  is_correct = correct;
}

void buzzer_patter() {
  if (!Error_sensor_p) {
    noTone(alarm);
    return;
  }
  if (millis() - buzzerTimer >= 500) {
    if (buzzerOn) {
      noTone(alarm);
    } else {
      tone(alarm, 1000);
    }
    buzzerOn = !buzzerOn;
    buzzerTimer = millis();
  }
}

void lcd_scroll_patter() {
  if (scrolling) {
    if (millis() - lcdScrollTimer >= lcdScrollDelay) {
      lcd_1.scrollDisplayLeft();
      lcdScrollTimer = millis();
    }
  }
}

// ----------- LOOP ------------
void loop() {
  buzzer_patter();
  lcd_scroll_patter();

  if (is_close()) {
    char pressed_key = keybo.getKey();
    led_patter(red_led, 500);

    switch (current_state) {
    case 0:
      update_string(pressed_key);
      break;
    case 1:
      update_string(pressed_key);
      break;
    case 2:
      update_string(pressed_key);
      break;
    case 3:
      update_string(pressed_key);
      break;

    case 4:
      if (prov == password) {
        digitalWrite(green_led, HIGH);
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("ACCESO PERMITIDO :D");
        reset_states(0, "", 0, true);
        scrolling = false;
        delay(5000);
        digitalWrite(green_led, LOW);
        lcd_1.clear();
      } else {
        reset_states(0, "", try_cnt + 1, false);
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("CONTRASENA ERRADA");
        lcd_1.setCursor(0, 1);
        lcd_1.print("Ingrese otra vez");
        scrolling = true; // activa scroll solo cuando hay error
      }
      if (try_cnt == 3) {

        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.println("Muchos intentos!");
        scrolling = true;
        reset_states(0, "", 0, true);
        delay(10000);
      }
      break;
    }
  } else {
    digitalWrite(red_led, LOW);
    reset_states(0, "", 0, true);
    scrolling = false;
  }
  delay(10);
}