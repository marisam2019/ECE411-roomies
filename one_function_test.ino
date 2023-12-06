const int LED_OUTPUT_RED = 12;
const int LED_OUTPUT_GREEN = 13;
const int LED_OUTPUT_BLUE = 27;
const int BUTTON_PIN1 = A1;
const int BUTTON_PIN2 = A3;
const int LED_OUTPUT_BUZZ = 33;
const int clk = 100;

//int count;
int focus = 2;
int short_break = 1;
int long_break = 3;

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

void countDown( int time ) {
  for (uint16_t minutes = time; minutes >= 0; minutes--) {
    for (uint16_t seconds = 59; seconds >= 0; seconds--) {
      int displayValue = minutes * 100 + seconds;
      matrix.println(displayValue);
      matrix.writeDisplay();

      if (time == focus) {
        digitalWrite(LED_OUTPUT_RED, HIGH);
      }
      else if (time == short_break) {
        digitalWrite(LED_OUTPUT_GREEN, HIGH);
      }
      else if (time == long_break) {
        digitalWrite(LED_OUTPUT_BLUE, HIGH);
      }

      blinkColon = !blinkColon;
      matrix.drawColon(blinkColon);

      matrix.writeDisplay();
      delay(clk);

      if (seconds == 0) {
        if (minutes != 0){
          minutes--;
        }
        else if ((minutes == 0) && (seconds == 0)){
          digitalWrite(LED_OUTPUT_RED, LOW);
          digitalWrite(LED_OUTPUT_GREEN, LOW);
          digitalWrite(LED_OUTPUT_BLUE, LOW);

          return;
        }
        seconds = 60;
      }
    }
  }
}

void loop() {
  countDown(focus);
  delay(500);
  countDown(short_break);
  delay(500);
  countDown(long_break);
  delay(500);
}