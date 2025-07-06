// Drives a single servo between 0 and 180 degrees
// https://msraynsford.blogspot.com/2025/07/making-move.html
// https://www.donteatpuzzles.com/

#include <Servo.h>

const uint8_t servoPin = D5;
Servo myservo;

void setup() {
  myservo.attach(servoPin, 500, 2500);
}

void loop() {
  myservo.writeMicroseconds(500);
  delay(500);

  myservo.writeMicroseconds(2500);
  delay(500);
}
