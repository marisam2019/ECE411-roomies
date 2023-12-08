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

unsigned long clk = 10;             // adjust countdown time in milliseconds
int focus = 25;                      // length of focus mode        
int short_break = 5;                // length of short break
int long_break = 30;                 // length of long break

bool blinkColon = true;
bool switch_pp = false;             // false = pause, true = play
bool resume = false;
bool skip_flag = false;

int minutesG = 25;                   // global variable minutes
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
  pinMode(BUTTON_PIN1, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN2, INPUT_PULLDOWN);

  attachInterrupt(BUTTON_PIN1, playPause, FALLING);    // play/pause interrupt
  attachInterrupt(BUTTON_PIN2, skipState, FALLING);    // skip interrupt (future)
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
  blinkColon = true;
  
  return;
}

void skipState() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime < 200){       // debounce
    return;
  }
  lastInterruptTime = interruptTime;

  Serial.println(countG);
  //minutesG = 0;
  //secondsG = 0;
  //skip_flag = skip_flag;

  skip_flag = true;
  switch_pp = false;
  blinkColon = true;

  digitalWrite(LED_OUTPUT_RED, LOW);
  digitalWrite(LED_OUTPUT_GREEN, LOW);
  digitalWrite(LED_OUTPUT_BLUE, LOW);
  
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

// resume countdown from saved time
void countDownResume() {
  // Serial.println(minutesG, secondsG);
  
  for (uint16_t minutes = minutesG; minutes >= 0; minutes--) {

    for (uint16_t seconds = secondsG; seconds >= 0; seconds--) {

      // saves current countdown time
      if (switch_pp == false){
        minutesG = minutes;
        secondsG = seconds;

        return;
      }
      if (skip_flag == true){
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

  digitalWrite(LED_OUTPUT_BUZZ, HIGH);
  waitTime(millis(), 100);
  digitalWrite(LED_OUTPUT_BUZZ, LOW);

  return;
}

void countDown ( int time ) {
    //Serial.println(time);

  // if (time == focus) {
  //   digitalWrite(LED_OUTPUT_RED, HIGH);
  // }
  // else if (time == short_break) {
  //   digitalWrite(LED_OUTPUT_GREEN, HIGH);
  // }
  // else if (time == long_break) {
  //   digitalWrite(LED_OUTPUT_BLUE, HIGH);
  // }
  
  for (uint16_t minutes = time - 1; minutes >= 0; minutes--) {

    for (uint16_t seconds = 59; seconds >= 0; seconds--) {

      if (switch_pp == false){
        minutesG = minutes;
        secondsG = seconds;

        return;
      }
      if (skip_flag == true){
        return;
      }


      int displayValue = minutes * 100 + seconds;
      matrix.println(displayValue);
      matrix.writeDisplay();

      blinkColon = !blinkColon;
      matrix.drawColon(blinkColon);
      matrix.writeDisplay();

      // wait clk
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

  digitalWrite(LED_OUTPUT_BUZZ, HIGH);
  waitTime(millis(), 100);
  digitalWrite(LED_OUTPUT_BUZZ, LOW);

  return;
}

void loop() {
  if (switch_pp == false) {

    if (skip_flag == true){
      skip_flag = false;
      countG = countG + 1;

      if (countG > 8){
       countG = 1;
      }

      secondsG = 0;

      switch (countG) {
        case 1:
        case 3:
        case 5:
        case 7:
          digitalWrite(LED_OUTPUT_RED, HIGH);
          minutesG = focus;
          break;

        case 2:
        case 4:
        case 6:
          digitalWrite(LED_OUTPUT_GREEN, HIGH);
          minutesG = short_break;
          break;
        
        case 8:
          digitalWrite(LED_OUTPUT_BLUE, HIGH);
          minutesG = long_break;
          // countG = 1;
          break;

        default:
          Serial.println("error cases");
          break;
      }
    }

    int displayValue = minutesG * 100 + secondsG;

    matrix.println(displayValue);
    matrix.writeDisplay();

    matrix.drawColon(blinkColon);
    matrix.writeDisplay();

    waitTime( millis(), 5 );
  }

  else if (resume == true){
    waitTime( millis(), 5 );

      switch (countG) {
        case 1:
        case 3:
        case 5:
        case 7:
          digitalWrite(LED_OUTPUT_RED, HIGH);
          break;

        case 2:
        case 4:
        case 6:
          digitalWrite(LED_OUTPUT_GREEN, HIGH);
          break;
        
        case 8:
          digitalWrite(LED_OUTPUT_BLUE, HIGH);
          break;

        default:
          Serial.println("error cases");
          break;
      }

    countDownResume();

    if (switch_pp == false){
      //:)
    }
    else
    {
      resume = false;

      countG = countG + 1;
      digitalWrite(LED_OUTPUT_RED, LOW);
      digitalWrite(LED_OUTPUT_GREEN, LOW);
      digitalWrite(LED_OUTPUT_BLUE, LOW);
    }
  }

  // Serial.println("-");
  else if (switch_pp == true) {
    waitTime( millis(), 5 );

    // Serial.println("~");
    for (int count = countG; count < 9; count++) {
      // Serial.println("|");
      
      switch (count){
        case 1:
        case 3:
        case 5:
        case 7:
          digitalWrite(LED_OUTPUT_RED, HIGH);
          countDown(focus);
          break;

        case 2:
        case 4:
        case 6:
          digitalWrite(LED_OUTPUT_GREEN, HIGH);
          countDown(short_break);
          break;
        
        case 8:
          digitalWrite(LED_OUTPUT_BLUE, HIGH);
          countDown(long_break);
          break;

        default:
          Serial.println("error cases");
          break;
      }

      countG = count;
      if (switch_pp == false){
        break;
      }
      if (skip_flag == true){
        skip_flag = false;

      }
    } //end of for loop

    if (switch_pp == true){
      countG = 1;
    }
  }
  else{
    Serial.println("error loop");
  }

}