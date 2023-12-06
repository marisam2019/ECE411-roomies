#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_7segment matrix = Adafruit_7segment();

const int buttonPlayPause = A1;
const int buttonSkip = A3;
const int ledFocus = 12;
const int ledShortBreak = 13;
const int ledLongBreak = 27;
//const int buzzer = 33

const int pomodoroDuration = 25 * 60;  // 25 minutes in seconds
const int shortBreakDuration = 5 * 60;  // 5 minutes in seconds
const int longBreakDuration = 30 * 60;  // 30 minutes in seconds

unsigned long previousMillis = 0;
int currentSessionDuration = pomodoroDuration;
int remainingTime = pomodoroDuration;
bool isPaused = false;
bool isNewSession = true;

void setup() {
  matrix.begin(0x70);  // Use the I2C address of your specific 7-segment display
  
  pinMode(buttonPlayPause, INPUT_PULLUP);
  pinMode(buttonSkip, INPUT_PULLUP);
  pinMode(ledFocus, OUTPUT);
  pinMode(ledShortBreak, OUTPUT);
  pinMode(ledLongBreak, OUTPUT);

  displaySessionType();
}

void loop() {
  handleButtons();
  
  if (!isPaused) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      updateTimer();
      previousMillis = currentMillis;
    }
  }
  
  displayTime();
}

void handleButtons() {
  if (digitalRead(buttonPlayPause) == LOW) {
    isPaused = !isPaused;
    if (isNewSession) {
      isNewSession = false;
      setSessionType();
    }
    delay(500);  // Debounce
  }

  if (digitalRead(buttonSkip) == LOW) {
    isNewSession = true;
    setNextSession();
    delay(500);  // Debounce
  }
}

void updateTimer() {
  if (remainingTime > 0) {
    remainingTime--;
  } else {
    isNewSession = true;
    setNextSession();
  }
}

void displayTime() {
  int minutes = remainingTime / 60;
  int seconds = remainingTime % 60;

  matrix.print(minutes * 100 + seconds, DEC);
  matrix.writeDisplay();
}

void setNextSession() {
  if (currentSessionDuration == pomodoroDuration) {
    currentSessionDuration = shortBreakDuration;
  } else if (currentSessionDuration == shortBreakDuration) {
    currentSessionDuration = longBreakDuration;
  } else {
    currentSessionDuration = pomodoroDuration;
  }

  setSessionType();
  remainingTime = currentSessionDuration;
}

void setSessionType() {
  digitalWrite(ledFocus, LOW);
  digitalWrite(ledShortBreak, LOW);
  digitalWrite(ledLongBreak, LOW);

  if (currentSessionDuration == pomodoroDuration) {
    digitalWrite(ledFocus, HIGH);
  } else if (currentSessionDuration == shortBreakDuration) {
    digitalWrite(ledShortBreak, HIGH);
  } else {
    digitalWrite(ledLongBreak, HIGH);
  }
}

void displaySessionType() {
  matrix.print(8888, DEC);  // Display "SESS" to indicate session type
  matrix.writeDisplay();
  delay(2000);
}
