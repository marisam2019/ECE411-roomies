/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

const int LED_OUTPUT_BLUE = 27; //470ohm
const int LED_OUTPUT_GREEN = 13; //4.3kkohm
const int LED_OUTPUT_RED = 12; //470ohm
const int LED_OUTPUT_BUZZ = 33; //buzzer
const int BUTTON_PIN1 = A1;
const int BUTTON_PIN2 = A3;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_OUTPUT_BLUE, OUTPUT);
  pinMode(LED_OUTPUT_GREEN, OUTPUT);
  pinMode(LED_OUTPUT_RED, OUTPUT);
  pinMode(LED_OUTPUT_BUZZ, OUTPUT);
  pinMode(BUTTON_PIN1, INPUT);
  pinMode(BUTTON_PIN2, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  if (digitalRead(BUTTON_PIN1) == HIGH) {
    digitalWrite(LED_OUTPUT_BLUE, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_OUTPUT_GREEN, HIGH);
    delay(2000);
    digitalWrite(LED_OUTPUT_BLUE, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_OUTPUT_GREEN, LOW);
  }
  if (digitalRead(BUTTON_PIN2) == HIGH) {
    digitalWrite(LED_OUTPUT_RED, HIGH);
    digitalWrite(LED_OUTPUT_BUZZ, HIGH);
    delay(500);
    digitalWrite(LED_OUTPUT_BUZZ, LOW);
    delay(1500);
    digitalWrite(LED_OUTPUT_RED, LOW);
  }

  else{
    digitalWrite(LED_OUTPUT_GREEN, LOW);
    digitalWrite(LED_OUTPUT_BLUE, LOW);
    digitalWrite(LED_OUTPUT_BUZZ, LOW);
    digitalWrite(LED_OUTPUT_RED, LOW);
  }

}
