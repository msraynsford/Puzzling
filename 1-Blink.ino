// A sample code to blink the built in LED using the FastLED Macro EVERY_N_MILLISECONDS
// Further details can be found at https://msraynsford.blogspot.com/2025/05/how-not-to-blink-led.html
// https://www.donteatpuzzles.com/

#include <FastLED.h>

#define INTERVAL  1000

const uint8_t ledPin = LED_BUILTIN;

bool ledState = false;

uint8_t brightness = 20;   // Set the led for half brightness.

void setup() {
  pinMode(ledPin, OUTPUT);      // Initialise the LED pin as an output
}

void loop() {
  // Every second turn the LED On/Off
  EVERY_N_MILLISECONDS(INTERVAL) {        
    ledState = !ledState;                 // Toggle the state of the LED
    
    if(ledState) {
      analogWrite(ledPin, 255 - brightness);   // Turn the led on
    } else {
      analogWrite(ledPin, 255);                // Turn the led off
    }
  }
}
