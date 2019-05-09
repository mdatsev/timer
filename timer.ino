#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int BUTTON1_PIN = 52;
const int BUTTON2_PIN = 53;

bool pressed(bool direction) {
  static int debounceTime = 0;
  static bool lastState = false;
  static bool held = false;
  bool reading = digitalRead(BUTTON2_PIN) == HIGH && digitalRead(BUTTON1_PIN) == HIGH;

  bool ret = false;

  if (millis() - debounceTime > 50) {
    if (reading == direction) {
      ret = held ^ direction;
    }
    held = reading;
  }

  if (lastState != reading) {
    debounceTime = millis();
  }
  lastState = reading;
  return ret;
}

void setup() {
  //start serial connection
  Serial.begin(9600);
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void loop() {
  static unsigned long lastTime;
  static float time;
  static bool running = false;
  static int count = 1;
  if (running) {
    time = (millis() - lastTime) / 1000.;
    // End timer
    if (pressed(true)) {
      running = false;
      count = 2;
    }
  } else {
    if (pressed(false)) {
      // Start timer
      count--;
      if (count == 0) {
        lastTime = millis();
        running = true;
        count = 2;
      }
    }
  }
  char buffer[100];
  dtostrf(time, 8, 3, buffer);
  lcd.setCursor(0, 0);
  lcd.print(buffer);
}
