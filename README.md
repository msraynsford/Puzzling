
# Puzzling
A selection of code samples that form the building blocks of my puzzle boxes.
All of these code samples were made using the [Wemos/Lolin D1 development board](https://www.wemos.cc/en/latest/d1/d1_mini.html) and minimal additional hardware as required. These samples are not intended to be ESP8266 specific and should run on any arduino development board although alternate pin choices may need to be made.

The description and images to go alongside this code can be found in the tutorial series on my blog https://msraynsford.blogspot.com/search/label/Tutorial

 [1-Blink.ino](https://github.com/msraynsford/Puzzling/blob/main/1-Blink.ino)
 
 Blink the built in LED using the FastLED Macro EVERY_N_MILLISECONDS and the digitalWrite function.
 
 [2-Flicker.ino](https://github.com/msraynsford/Puzzling/blob/main/2-Flicker.ino)
 
 Control the brightness of the built in LED using the analogueWrite function.
 
 [3-DigitalRead.ino](https://github.com/msraynsford/Puzzling/blob/main/3-DigitalRead.ino)
 
 Read the value of a single digital input, monitoring to button press/release and hold count.

 [4-MultipleIO.ino](https://github.com/msraynsford/Puzzling/blob/main/4-MultipleIO.ino)
 
 Reads an array of digital input values and uses them to control the mode for the output led.

 [5-FadeLED.ino](https://github.com/msraynsford/Puzzling/blob/main/5-FadeLED.ino)

 Implements two different methods to create a fading cycle for an LED

 [6-BinaryPuzzle.ino](https://github.com/msraynsford/Puzzling/blob/main/6-BinaryPuzzle.ino)

 A simple puzzle that flashes a binary code and checks for the correct binary code response
