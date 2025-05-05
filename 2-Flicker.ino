// A sample code to control the built in LED with a variety of defined modes
// Off, On, PWM, Flicker, Blink
// Further details can be found at https://msraynsford.blogspot.com/2025/05/how-to-flicker-led.html
// https://www.donteatpuzzles.com/

#include <FastLED.h>

#define INTERVAL  25

enum class ledModes { Off,
                      On,
                      Blink,
                      PWM,
                      Flicker};

const uint8_t ledMAX = 32;
const uint8_t ledMIN = 0;

const uint8_t ledPin = LED_BUILTIN;

ledModes      ledMode = ledModes::Off;
uint8_t       ledValue = ledMAX;

uint8_t loopCount = 0;


void setup() {
  pinMode(ledPin, OUTPUT);      // Initialise the LED pin as an output

  // Set the kind of behaviour from the LED
  ledMode = ledModes::Flicker;  

  //ledMode = ledModes::Blink;
  
  //ledMode = ledModes::PWM;
  //ledValue = 16;
}

void loop() {
  EVERY_N_MILLISECONDS(INTERVAL) {        
    controlLights();
  }
}

void controlLights(void) {
  switch (ledMode) {
    case ledModes::Off:
      // Turn the PWM all the way off
      ledValue = ledMIN;
      break;

    case ledModes::On:              
      // Turn the PWM all the way on
      ledValue = ledMAX;
      break;

    case ledModes::PWM:
      // Nothing needs to be done for PWM
      break;

    case ledModes::Flicker:
      // Randomly select a value for the brightness
      ledValue = random(ledMAX);
      break;

    case ledModes::Blink:           
      // Check how many loops have elapsed and toggle on/off every second
      if(loopCount > 40) {
        ledValue = ledMAX - ledValue;
        loopCount = 0;
      }
      break;
  }

  // Send the LED value out to the ledPin
  analogWrite(ledPin, 255 - ledValue);

  // Count how many times this loop has be performed
  loopCount++;                          
}
