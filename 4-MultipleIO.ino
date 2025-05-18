// Uses two different input buttons to switch between
// the whole range of ledModes defined in this system
// Further details can be found at 
// https://msraynsford.blogspot.com/2025/05/multiple-buttons-and-leds.html
// https://www.donteatpuzzles.com/

#include <FastLED.h>

#define INTERVAL 25

const uint8_t numBtn = 2;
const uint8_t btnPin[] = { D1, D2 };

const uint8_t numLed = 1;
const uint8_t ledPin[] = { LED_BUILTIN };

enum class ledModes { Off,
                      On,
                      Blink,
                      PWM,
                      Flicker,
                      numModes };

const uint8_t numLedModes = (uint8_t)ledModes::numModes;

const uint8_t ledMAX = 32;
const uint8_t ledMIN = 0;


uint8_t btnCurValue[numBtn];
uint8_t btnOldValue[numBtn];
uint8_t btnCount[numBtn];

ledModes ledMode[] = { ledModes::Off };
uint8_t ledValue[numLed];

uint8_t loopCount = 0;

void setup() {
  setupInputs();
  setupLights();
}

void loop() {
  EVERY_N_MILLISECONDS(INTERVAL) {
    controlInputs();

    // Switch the led mode when the first button is pushed
    if (btnDown(0)) {
      // Increment the led mode by one and loop round after the last mode
      ledMode[0] = (ledModes)(((uint8_t)ledMode[0] + 1) % numLedModes);
     }

    if (btnDown(1)) {
      // Decrement the led mode by one and loop round.
      // Do this by adding the number of modes first, this prevents the possibility of a negative number
      ledMode[0] = (ledModes)(((uint8_t)ledMode[0] + numLedModes - 1) % numLedModes);
    }

    controlLights();
  }
}

void setupInputs() {
  for (uint8_t i = 0; i < numBtn; i++) {
    pinMode(btnPin[i], INPUT_PULLUP);  // Initialise the btn pin as an input
  }
}

void setupLights() {
  for (uint8_t i = 0; i < numLed; i++) {
    pinMode(ledPin[i], OUTPUT);  // Initialise the LED pin as an output
  }
}


void controlInputs() {
  for (uint8_t i = 0; i < numBtn; i++) {
    btnOldValue[i] = btnCurValue[i];
    btnCurValue[i] = digitalRead(btnPin[i]);

    if (btnUp(i)) {
      // The button has just been released. Reset the counter
      btnCount[i] = 0;
    } else if (!btnCurValue[i]) {
      // Add to the count for each loop the butotn is held down
      // Prevent the counter from overflowing by limiting to 250
      if (btnCount[i] < 250) {
        btnCount[i]++;
      }
    }
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
        if (loopCount > 40) {
          ledValue[i] = ledMAX - ledValue[i];
          loopCount = 0;
        }
        break;
    }

    // Send the LED value out to the ledPin
    analogWrite(ledPin[i], 255 - ledValue[i]);
  }

  // Count how many times this loop has be performed
  loopCount++;
}

bool btnUp(uint8_t btnNo) {
  return !btnOldValue[btnNo] & btnCurValue[btnNo];
}

bool btnDown(uint8_t btnNo) {
  return btnOldValue[btnNo] & !btnCurValue[btnNo];
}
