#ifndef LIGHTCONTROL_H
#define LIGHTCONTROL_H

const uint8_t fadeLUT[] = {
  0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 2, 2, 3, 3, 4, 5,
  6, 7, 8, 9, 10, 11, 13, 15,
  17, 19, 21, 24, 26, 29, 31, 31
};
const uint8_t numfadeLUTs = ARRAY_LENGTH(fadeLUT);


const uint8_t ledPin[] = { LED_BUILTIN };
const uint8_t numLeds = ARRAY_LENGTH(ledPin);

enum class ledModes { Off,
                      On,
                      Blink,
                      PWM,
                      Flicker,
                      Fade,
                      numModes };
const uint8_t numLedModes = (uint8_t)ledModes::numModes;
ledModes ledMode[] = { ledModes::Off };


const uint8_t ledMAX = 32;
const uint8_t ledMIN = 0;

const uint8_t loopMAX = ledMAX * 2;

uint8_t ledValue[numLeds];
uint8_t loopCount = 0;


void setupLights() {
  for (uint8_t i = 0; i < numLeds; i++) {
    pinMode(ledPin[i], OUTPUT);  // Initialise the LED pins as outputs
  }
}


void controlLights() {
  for (uint8_t i = 0; i < numLeds; i++) {
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

#endif