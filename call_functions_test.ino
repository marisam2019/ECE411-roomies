const int LED_OUTPUT_RED = 12;
const int LED_OUTPUT_GREEN = 13;
const int LED_OUTPUT_BLUE = 27;
const int BUTTON_PIN1 = A1;
const int BUTTON_PIN2 = A3;
const int LED_OUTPUT_BUZZ = 33;
const int clk = 100;

//int count;
//int focusCount = 0;
//int shortBreakCount = 0;
//int longBreakCount = 0;

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  matrix.begin(0x70);
  pinMode(LED_OUTPUT_RED, OUTPUT);
  pinMode(LED_OUTPUT_GREEN, OUTPUT);
  pinMode(LED_OUTPUT_BLUE, OUTPUT);
}

    bool blinkColon = false;

void focus() {
  for (uint16_t minutes = 2; minutes >= 0; minutes--) {
    for (uint16_t seconds = 59; seconds >= 0; seconds--) {
      int displayValue = minutes * 100 + seconds;
      matrix.println(displayValue);
      matrix.writeDisplay();
      digitalWrite(LED_OUTPUT_RED, HIGH);

      blinkColon = !blinkColon;
      matrix.drawColon(blinkColon);

      matrix.writeDisplay();
      delay(clk);

      if (seconds == 0) {
        if (minutes != 0){
          minutes--;
        }
        else if ((minutes == 0) && (seconds == 0)){
          return;
        }
        seconds = 60;
      }
    }
  }
}
void short_break() {
  for (uint16_t minutes = 1; minutes >= 0; minutes--) {
    for (uint16_t seconds = 59; seconds >= 0; seconds--) {
      int displayValue = minutes * 100 + seconds;
      matrix.println(displayValue);
      matrix.writeDisplay();
      digitalWrite(LED_OUTPUT_GREEN, HIGH);

      blinkColon = !blinkColon;
      matrix.drawColon(blinkColon);

      matrix.writeDisplay();
      delay(clk);

      if (seconds == 0) {
        if (minutes != 0){
          minutes--;
        }
        else if ((minutes == 0) && (seconds == 0)){
          return;
        }
        seconds = 60;
      }
    }
  }
}
void long_break() {
  for (uint16_t minutes = 3; minutes >= 0; minutes--) {
    for (uint16_t seconds = 59; seconds >= 0; seconds--) {
      int displayValue = minutes * 100 + seconds;
      matrix.println(displayValue);
      matrix.writeDisplay();
      digitalWrite(LED_OUTPUT_BLUE, HIGH);

      blinkColon = !blinkColon;
      matrix.drawColon(blinkColon);

      matrix.writeDisplay();
      delay(clk);

      if (seconds == 0) {
        if (minutes != 0){
          minutes--;
        }
        else if ((minutes == 0) && (seconds == 0)){
          return;
        }
        seconds = 60;
      }
    }
  }
}

void loop() {
  focus();
  digitalWrite(LED_OUTPUT_RED, LOW);
  delay(500);
  short_break();
  digitalWrite(LED_OUTPUT_GREEN, LOW);
  delay(500);
  long_break();
  digitalWrite(LED_OUTPUT_BLUE, LOW);
  delay(500);
}