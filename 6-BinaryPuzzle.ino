// Uses two input buttons and two leds to create a 
// puzzle that flashes a code in binary and looks
// for the correct binary response
// Further details can be found at
// https://msraynsford.blogspot.com/2025/06/binary-puzzle.html
// https://www.donteatpuzzles.com/

#include <FastLED.h>

#define INTERVAL 25

const uint8_t numBtns = 2;
const uint8_t btnPin[] = { D1, D2 };

const uint8_t numLeds = 2;
const uint8_t ledPin[] = { D6, D5 };

enum class ledModes { Off,
                      On,
                      PWM,
                      Flicker,
                      Blink,
                      Fade,
                      numModes };

const uint8_t numLedModes = (uint8_t)ledModes::numModes;

const uint8_t ledMAX = 32;
const uint8_t ledMIN = 0;

const uint8_t loopMAX = ledMAX * 2;

const uint8_t fadeLUT[] = {
  0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 2, 2, 3, 3, 4, 5,
  6, 7, 8, 9, 10, 11, 13, 15,
  17, 19, 21, 24, 26, 29, 31, 31
};

const uint8_t numCodes = 10;
const uint8_t code[][2] = { { 0b10101001, 0b00111001 },
                            { 0b11100010, 0b00000111 },
                            { 0b00100111, 0b01111000 },
                            { 0b00000111, 0b10101110 },
                            { 0b11010000, 0b10001011 },
                            { 0b00111111, 0b01001000 },
                            { 0b01110011, 0b00010110 },
                            { 0b01100001, 0b00010101 },
                            { 0b10111000, 0b00010001 },
                            { 0b01110001, 0b11001101 } };

uint8_t codeNo = 0;

uint8_t btnCurValue[numBtns];
uint8_t btnOldValue[numBtns];
uint8_t btnCount[numBtns];

ledModes ledMode[] = { ledModes::Off, ledModes::Off };
uint8_t ledValue[numLeds];

uint8_t loopCount = 0;

enum class gameStates { DisplayCode,
                        InputCode,
                        CheckCode };

gameStates gameState = gameStates::DisplayCode;

uint8_t input = 0;
uint8_t bitCount = 0;
uint32_t gameCount = 0;

void setup() {
  randomSeed(2);
  setupGame();
  setupInputs();
  setupLights();
}

void loop() {
  EVERY_N_MILLISECONDS(INTERVAL) {
    controlInputs();
    controlGame();
    controlLights();
  }
}


void controlGame() {
  static uint8_t gameCount = 0;

  switch (gameState) {
    // Blink the led pattern out and wait for a button input.
    case gameStates::DisplayCode:
      // Flash the led's to show the number of the code being requested
      displayCode();

      // If either button is pushed then immediately jump to the input state
      for (uint8_t i = 0; i < numBtns; i++) {
        if (btnDown(i)) {
          gameState = gameStates::InputCode;
          gameCount = 0;
          
          // Record this as the first button pushed
          input = i;
          bitCount = 1;
        }
      }
      break;

    // Record the next eight button pushes, flashing the leds as the buttons are pushed
    case gameStates::InputCode:

      for (uint8_t i = 0; i < numBtns; i++) {
        // If either button is pushed then record the value and increment the bitCount
        if (btnDown(i)) {
          input = (input << 1) + i;
          bitCount++;
          gameCount = 0;
        }

        // Turn on the corresponding LED while the button is held down.
        if (btnCurValue[i]) {
          ledMode[i] = ledModes::Off;
        } else {
          ledMode[i] = ledModes::On;
        }
      }

      // When eight buttons have been pushed move to the next state to check the code and display the result
      if (bitCount == 8) {
        gameState = gameStates::CheckCode;
        gameCount = 0;
      }

      // If the game times out, move to the next stage to flash an error before resetting
      if (gameCount < 200) {
        gameCount++;
      } else {
        gameState = gameStates::CheckCode;
        gameCount = 0;
      }

      break;

    // Check to see if the input code is the right answer and respond accordingly
    case gameStates::CheckCode:
      // If the code is correct then switch the LEDs into a fading cycle
      if (input == code[codeNo][1]) {
        for (uint8_t i = 0; i < numLeds; i++)
          ledMode[i] = ledModes::Fade;
      }

      // If the code is incorrect, flicker the LED's for a duration
      // Select a new code and reset the system to start over again
      else {
        if (gameCount < 40) {
          for (uint8_t i = 0; i < numLeds; i++)
            ledMode[i] = ledModes::Flicker;

        } else if (gameCount < 80) {
          for (uint8_t i = 0; i < numLeds; i++)
            ledMode[i] = ledModes::Off;

        } else {
          gameState = gameStates::DisplayCode;
          setupGame();
        }

        gameCount++;
      }
      break;
  }
}

void setupGame() {
  // Reset some of the global values
  gameCount = 0;
  bitCount = 0;

  // Switch to a new code
  uint8_t newCodeNo = 0;

  do {
    newCodeNo = random(numCodes);
  } while (newCodeNo == codeNo);

  codeNo = newCodeNo;
}

uint32_t displayCount = 0;

void displayCode() {
  const uint8_t bitDelay = 24;   // A delay between bits
  const uint8_t byteDelay = 12;  // Wait for half a byte before repeating

  // Work out which bit to display
  uint8_t bitNo = gameCount / bitDelay;

  // Turn off both LEDs before working out which one to turn back on
  ledMode[0] = ledModes::Off;
  ledMode[1] = ledModes::Off;

  // Only update the LED for the first eight bits
  if (bitNo < 8) {

    // Turn the LED on for the last 3/4 of the cycle, this creates a blink between bits.
    if (gameCount % bitDelay > bitDelay / 4) {

      // Work out if the new bit is a 0 or 1
      // Read the bits from left to right
      uint8_t bitValue = (code[codeNo][0] >> (7 - bitNo)) & 1;
      ledMode[bitValue] = ledModes::On;
    }
  }

  // Add to the count and loop
  gameCount++;
  gameCount %= bitDelay * byteDelay;
}

void setupInputs() {
  for (uint8_t i = 0; i < numBtns; i++) {
    pinMode(btnPin[i], INPUT_PULLUP);  // Initialise the btn pin as an input
  }
}

void setupLights() {
  for (uint8_t i = 0; i < numLeds; i++) {
    pinMode(ledPin[i], OUTPUT);  // Initialise the LED pin as an output
  }
}


void controlInputs() {
  for (uint8_t i = 0; i < numBtns; i++) {
    btnOldValue[i] = btnCurValue[i];
    btnCurValue[i] = digitalRead(btnPin[i]);

    if (btnUp(i)) {
      // The button has just been released. Reset the counter
      btnCount[i] = 0;
    } else if (!btnCurValue[i]) {
      // Add to the count for each loop the button is held down
      // Prevent the counter from overflowing by limiting to 250
      if (btnCount[i] < 250) {
        btnCount[i]++;
      }
    }
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

  // Count how many times this loop has be performed
  loopCount++;
  loopCount %= loopMAX;
}

bool btnUp(uint8_t btnNo) {
  return !btnOldValue[btnNo] & btnCurValue[btnNo];
}

bool btnDown(uint8_t btnNo) {
  return btnOldValue[btnNo] & !btnCurValue[btnNo];
}
