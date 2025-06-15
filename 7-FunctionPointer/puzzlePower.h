#ifndef PUZZLEPOWER_H
#define PUZZLEPOWER_H

// Define all three possible states for the knife switch
enum class switchStates { Up = 1,
                          Down = 2,
                          Floating = 3 };

// Track the initial state and current states of the switch
switchStates switchOldState;
switchStates switchCurState;

// Convert the two inputs into a single knife switch value
switchStates getSwitchState() {
  return (switchStates)((btnCurValue[0] << 0) + (btnCurValue[1] << 1));
}

// A simple puzzle where the player just has to flip the switch from it's initial position
// to it's opposite position
bool puzzlePower(bool setup) {
  bool complete = false;

  // Code to perform once at the start of each puzzle
  if (setup) {
    Serial.printf("Starting Puzzle: Power\n");

    // Record the initial position of the switch
    ledMode[0] = ledModes::Off;
    switchOldState = getSwitchState();
  }

  // Code to actually play the puzzle
  else {
    // If the switch value has changed process the new state
    if (switchCurState != getSwitchState()) {
      // Record the new value
      switchCurState = getSwitchState();

      // Print the new value for debug
      switch (switchCurState) {
        case switchStates::Up: Serial.printf("Switch State: Up\n"); break;
        case switchStates::Down: Serial.printf("Switch State: Down\n"); break;
        case switchStates::Floating: Serial.printf("Switch State: Floating\n"); break;
      }

      // Check to see if the switch is in the correct position to move on
      complete = ((switchCurState != switchStates::Floating) && (switchCurState != switchOldState));
    }
  }

  // Code to perform once to tidy up at the end of the puzzle
  if (complete) {
    ledMode[0] = ledModes::On;
    Serial.printf("Puzzle: Power Completed\n");
  }

  return complete;
}


#endif