#ifndef IOCONTROL_H
#define IOCONTROL_H


const uint8_t btnPin[] = { D1, D2 };
const uint8_t numBtns = ARRAY_LENGTH(btnPin);

uint8_t btnCurValue[numBtns];
uint8_t btnOldValue[numBtns];
uint8_t btnCount[numBtns];


void setupIO() {
  for (uint8_t i = 0; i < numBtns; i++) {
    pinMode(btnPin[i], INPUT_PULLUP);  // Initialise the btn pin as an input
  }
}


bool btnUp(uint8_t btnNo) {
  return !btnOldValue[btnNo] & btnCurValue[btnNo];
}


bool btnDown(uint8_t btnNo) {
  return btnOldValue[btnNo] & !btnCurValue[btnNo];
}


void controlIO() {
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

    // if (btnDown(i)) {
    //   Serial.printf("Button Down %d\n", i);
    // }
  }
}



#endif