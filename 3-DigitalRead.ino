
// A sample code to read a switch into a digital pin
// The system is capable of detecting button press and release
// and keeps track of how long the button has been held down for
// Further details can be found at 
// https://msraynsford.blogspot.com/2025/05/how-to-read-switch.html
// https://www.donteatpuzzles.com/

#include <FastLED.h>

#define INTERVAL 25

const uint8_t btnPin = D1;
const uint8_t ledPin = LED_BUILTIN;

uint8_t btnCurValue;
uint8_t btnOldValue;
uint8_t btnCount = 0;

void setup() {
  Serial.begin(115200);

  pinMode(btnPin, INPUT_PULLUP);  // Initialise the btn pin as an input
  pinMode(ledPin, OUTPUT);  // Initialise the LED pin as an output
}

void loop() {
  EVERY_N_MILLISECONDS(INTERVAL) {
    btnOldValue = btnCurValue;
    btnCurValue = digitalRead(btnPin);

    if (btnUp()) {
      // The button has just been released. Reset the counter
      btnCount = 0;
    } else if (!btnCurValue) {
      // Add to the count for each loop the button is held down
      // Prevent the counter from overflowing by limiting to 250
      if (btnCount < 250) {
        btnCount++;
      }
    }

    // Turn the LED on if the button is held down for longer than 1 second
    digitalWrite(ledPin, !(btnCount > 40));
  }
}
bool btnUp() {
  return !btnOldValue & btnCurValue;
}

bool btnDown() {
  return btnOldValue & !btnCurValue;
}
