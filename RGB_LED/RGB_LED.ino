#include <stdio.h>

int redPin = 3;     // Red RGB pin -> D3
int greenPin = 5;   // Green RGB pin -> D5
int bluePin = 27;    // Blue RGB pin -> D6

void setup() {
  pinMode(redPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(greenPin, OUTPUT);

}

void loop() {

  digitalWrite(redPin, HIGH);
  delay(1000);
  digitalWrite(redPin, LOW);
  delay(1000);
  digitalWrite(greenPin, HIGH);
  delay(1000);
  digitalWrite(greenPin, LOW);
  delay(1000);
  digitalWrite(bluePin, HIGH);
  delay(1000);
  digitalWrite(bluePin, LOW);
  delay(1000);
  digitalWrite(redPin, HIGH);
  delay(1000);
  digitalWrite(, LOW);
  delay(1000);
  digitalWrite(greenPin, HIGH);
  delay(1000);
  delay(10);
}