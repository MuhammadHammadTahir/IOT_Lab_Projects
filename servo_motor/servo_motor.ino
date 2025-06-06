#include <ESP32Servo.h>

Servo myServo;

void setup() {
  myServo.attach(27);  // Connect signal wire to GPIO 27
}

void loop() {
  // Move to 90 degrees
  myServo.write(90);
  delay(1000); // wait for 1 second

  // Move back to 0 degrees
  myServo.write(0);
  delay(1000); // wait for 1 second
}
