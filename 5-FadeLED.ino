// Implements two different methods for fading the brightness of
// an LED, direct linear control and non linear control using
// a Look Up Table
// Further details can be found at
// https://msraynsford.blogspot.com/2025/05/how-not-to-fade-led.html
// https://www.donteatpuzzles.com/

#include <FastLED.h>

#define INTERVAL 25

const uint8_t numLed = 3;
const uint8_t ledPin[] = { LED_BUILTIN, D5, D6 };

enum class ledModes { Off,
                      On,
                      Blink,
                      PWM,
                      Flicker,
                      Fade,
                      FadeLUT,
                      numModes };

const uint8_t numLedModes = (uint8_t)ledModes::numModes;

const uint8_t ledMAX = 32;
const uint8_t ledMIN = 0;

const uint8_t loopMAX = ledMAX * 2;

const uint8_t fadeLUT[] = {
  0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 2, 2, 3, 3, 4, 5,
  6, 7, 8, 9, 10, 11, 13, 15,
  17, 19, 21, 24, 26, 29, 31, 31 };

ledModes ledMode[] = { ledModes::Blink, ledModes::Fade, ledModes::FadeLUT };
uint8_t ledValue[numLed];

uint8_t loopCount = 0;

void setup() {
  setupLights();
}

void loop() {
  EVERY_N_MILLISECONDS(INTERVAL) {
    controlLights();
  }
}


void setupLights() {
  for (uint8_t i = 0; i < numLed; i++) {
    pinMode(ledPin[i], OUTPUT);  // Initialise the LED pin as an output
  }
}


void controlLights() {
  for (uint8_t i = 0; i < numLed; i++) {
    switch (ledMode[i]) {
      case ledModes::Off:
        // Turn the PWM all the way off
        ledValue[i] = ledMIN;
        break;

      case ledModes::On:
        // Turn the PWM all the way on
        ledValue[i] = ledMAX;
        break;

      case ledModes::PWM:
        // Nothing needs to be done for PWM
        break;

      case ledModes::Flicker:
        // Randomly select a value for the brightness
        ledValue[i] = random(ledMAX);
        break;

      case ledModes::Blink:
        // Check how many loops have elapsed and toggle on/off every second
        if (loopCount == 0) {
          ledValue[i] = ledMAX;
        } else if (loopCount == (loopMAX / 2)) {
          ledValue[i] = ledMIN;
        }
        break;

      case ledModes::Fade:
        if (loopCount < loopMAX / 2) {
          ledValue[i] = loopCount;
        } else {
          ledValue[i] = loopMAX - 1 - loopCount;
        }
        break;

      case ledModes::FadeLUT:
        if (loopCount < loopMAX / 2) {
          ledValue[i] = fadeLUT[loopCount];
        } else {
          ledValue[i] = fadeLUT[loopMAX - 1 - loopCount];
        } 
        break;
    }

    // Send the LED value out to the ledPin
    analogWrite(ledPin[i], 255 - ledValue[i]);
  }

  // Count how many times this loop has been performed
  loopCount++;
  loopCount %= loopMAX;
}
