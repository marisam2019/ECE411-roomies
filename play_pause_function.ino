#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

const int LED_OUTPUT_RED = 12;      // focus mode
const int LED_OUTPUT_GREEN = 13;    // short break
const int LED_OUTPUT_BLUE = 27;     // long break
const int BUTTON_PIN1 = A1;         // play/pause
const int BUTTON_PIN2 = A3;         // skip
const int LED_OUTPUT_BUZZ = 33;     // buzzer

unsigned long clk = 100;             // adjust countdown time in milliseconds
int focus = 2;                      // length of focus mode        
int short_break = 1;                // length of short break
int long_break = 3;                 // length of long break

bool blinkColon = false;
bool switch_pp = false;             // false = pause, true = play
bool resume = false;

int minutesG = 0;                   // global variable minutes
int secondsG = 0;                   // global variable seconds
int countG = 1;                     // global variable count


//prototypes
//void playPause (void);

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  matrix.begin(0x70);
  pinMode(LED_OUTPUT_RED, OUTPUT);
  pinMode(LED_OUTPUT_GREEN, OUTPUT);
  pinMode(LED_OUTPUT_BLUE, OUTPUT);
  pinMode(LED_OUTPUT_BUZZ, OUTPUT);
  pinMode(BUTTON_PIN1, INPUT);
  pinMode(BUTTON_PIN2, INPUT);

  attachInterrupt(BUTTON_PIN1, playPause, FALLING);    // play/pause interrupt
  //attachInterrupt(GPIOPin, ISR, Mode);                // skip interrupt (future)
}

// interrupt function that plays and pauses the timer (with debounce)
void playPause() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime < 200){       // debounce
    return;
  }
  lastInterruptTime = interruptTime;

  Serial.print(".");
  switch_pp = !switch_pp;
  if (switch_pp == true){
    resume = true;
  }
  
  return;
}

// wait for "clk" amount of milliseconds
void waitTime( unsigned long start_time, unsigned long clk ){
  unsigned long current_time = millis();
  // Serial.println(clk);
  // Serial.print("start time ");
  // Serial.println(start_time);

  // while fewer that "clk" milliseconds have passed since waitTime func called
  while (current_time - start_time < clk){
    current_time = millis();
    // wait
    // Serial.print("current time ");
    // Serial.println(current_time);

  }
  // Serial.println("returning from waitTime");
}

// 
void countDownResume() {
  Serial.println(minutesG, secondsG);
  
  for (uint16_t minutes = minutesG; minutes >= 0; minutes--) {

    for (uint16_t seconds = secondsG; seconds >= 0; seconds--) {

      if (switch_pp == false){
        minutesG = minutes;
        secondsG = seconds;

        return;
      }

      int displayValue = minutes * 100 + seconds;
      matrix.println(displayValue);
      matrix.writeDisplay();

      blinkColon = !blinkColon;
      matrix.drawColon(blinkColon);
      matrix.writeDisplay();

      // wait 1 sec
      unsigned long start_time = millis();
      waitTime(start_time, clk);

      if (seconds == 0){
        secondsG = 59;
        break;
      }
    }
    if (minutes == 0){
      break;
    }
  }

  //end of timer, turn off leds
  digitalWrite(LED_OUTPUT_RED, LOW);
  digitalWrite(LED_OUTPUT_GREEN, LOW);
  digitalWrite(LED_OUTPUT_BLUE, LOW);

  return;
}

void countDown ( int time ) {
    Serial.println(time);

  if (time == focus) {
    digitalWrite(LED_OUTPUT_RED, HIGH);
  }
  else if (time == short_break) {
    digitalWrite(LED_OUTPUT_GREEN, HIGH);
  }
  else if (time == long_break) {
    digitalWrite(LED_OUTPUT_BLUE, HIGH);
  }
  
  for (uint16_t minutes = time; minutes >= 0; minutes--) {

    for (uint16_t seconds = 59; seconds >= 0; seconds--) {

      if (switch_pp == false){
        minutesG = minutes;
        secondsG = seconds;

        return;
      }

      int displayValue = minutes * 100 + seconds;
      matrix.println(displayValue);
      matrix.writeDisplay();

      blinkColon = !blinkColon;
      matrix.drawColon(blinkColon);
      matrix.writeDisplay();

      // wait 1 sec
      unsigned long start_time = millis();
      waitTime(start_time, clk);

      if (seconds == 0){
        break;
      }
    }
    if (minutes == 0){
      break;
    }
  }

  //end of timer, turn off leds
  digitalWrite(LED_OUTPUT_RED, LOW);
  digitalWrite(LED_OUTPUT_GREEN, LOW);
  digitalWrite(LED_OUTPUT_BLUE, LOW);

  return;
}

void loop() {
  if (resume == true){
    countDownResume();
  }

  // Serial.println("-");
  if (switch_pp == true) {
    waitTime( millis(), 5 );

    // Serial.println("~");
    for (int count = countG; count < 9; count++) {
      // Serial.println("|");
      
      switch (count ){
        case 1:
        case 3:
        case 5:
        case 7:
          countDown(focus);
          break;

        case 2:
        case 4:
        case 6:
          countDown(short_break);
          break;
        
        case 8:
          countDown(long_break);
          break;

        default:
          Serial.println("error cases");
          break;
      }
      if (switch_pp == false){
        countG = count;
        break;
      }
    }
    if (switch_pp == true){
      countG = 1;
    }
  }

  else if (switch_pp == false) {
    int displayValue = minutesG * 100 + secondsG;
    matrix.println(displayValue);
    matrix.writeDisplay();

    matrix.drawColon(blinkColon);
    matrix.writeDisplay();

    waitTime( millis(), 5 );
  }
}