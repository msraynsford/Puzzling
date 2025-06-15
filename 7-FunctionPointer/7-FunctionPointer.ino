// Implements two different methods for fading the brightness of
// an LED, direct linear control and non linear control using
// a Look Up Table
// Further details can be found at
// https://msraynsford.blogspot.com/2025/05/how-not-to-fade-led.html
// https://www.donteatpuzzles.com/

#include <FastLED.h>
#include "constants.h"
#include "ioControl.h"
#include "lightControl.h"

#include "puzzle.h"
#include "puzzlePower.h"


const PuzzleFunction puzzles[] = { puzzlePower, puzzle };
const uint8_t numPuzzles = ARRAY_LENGTH(puzzles);

uint8_t puzzleNo = 0;

void setup() {
  Serial.begin(115200);
  Serial.printf("\nFunction Pointer Example\n");

  setupIO();
  setupLights();

  // Don't forget to set up the first game
  controlIO();
  puzzles[puzzleNo](true);
}


void loop() {
  EVERY_N_MILLISECONDS(INTERVAL) {
    controlIO();

    if (puzzles[puzzleNo](false)) {
      if (puzzleNo < numPuzzles) {
        puzzleNo++;

        Serial.printf("Switching to puzzle number %d\\%d\n", puzzleNo + 1, numPuzzles);
        puzzles[puzzleNo](true);
      }
    }

    controlLights();
  }
}
