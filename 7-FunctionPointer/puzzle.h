#ifndef PUZZLE_H
#define PUZZLE_H

bool puzzle(bool setup) {
  bool complete = false;

  // Code to perform once at the start of each puzzle
  if (setup) {
    Serial.printf("Starting Puzzle: Blank\n");
  }

  // Code to actually play the puzzle
  else {
  }

  // Code to perform once to tidy up at the end of the puzzle
  if (complete) {
    Serial.printf("Puzzle: Blank Completed\n");
  }

  return complete;
}


#endif