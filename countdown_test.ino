const int LED_OUTPUT_RED = 12;
const int clk = 100;

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
}

    bool blinkColon = false;

void loop() {

  for (uint16_t minutes = 24; minutes > 0; minutes--) {
    for (uint16_t seconds = 59; seconds >= 0; seconds--) {
      int displayValue = minutes * 100 + seconds;
      matrix.println(displayValue);
      matrix.writeDisplay();
      //delay(clk); //1000 for 1 second delay
      digitalWrite(LED_OUTPUT_RED, HIGH);

        blinkColon = !blinkColon;
        matrix.drawColon(blinkColon);

        matrix.writeDisplay();
        delay(clk);

      // Check if seconds have reached 00
      if (seconds == 0) {
        // Reset seconds to 59 and decrement minutes
        seconds = 60; // set to 60, so it becomes 59 in the next iteration
        minutes--;
      }
    }
  }
}
