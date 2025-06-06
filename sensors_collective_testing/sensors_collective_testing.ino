// Define the pin connected to the IR sensor
#define irSensorPin1 27 
#define irSensorPin2 26 
#define irSensorPin3 25 
#define irSensorPin4 33 
#define irSensorPin5 32 
#define irSensorPin6 35

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Set IR sensor pin as input
  pinMode(irSensorPin1, INPUT);
  pinMode(irSensorPin2, INPUT);
  pinMode(irSensorPin3, INPUT);
  pinMode(irSensorPin4, INPUT);
  pinMode(irSensorPin5, INPUT);
  pinMode(irSensorPin6, INPUT);
}

void loop() {
  // Read IR sensor
  int sensorValue1 = digitalRead(irSensorPin1);
  int sensorValue2 = digitalRead(irSensorPin2);
  int sensorValue3 = digitalRead(irSensorPin3);
  int sensorValue4 = digitalRead(irSensorPin4);
  int sensorValue5 = digitalRead(irSensorPin5);
  int sensorValue6 = digitalRead(irSensorPin6);

  // Check if IR sensor is detecting something
  if (sensorValue1 == LOW) {
    Serial.println("Object Detected at sensor 1!");
  } else {
    Serial.println("No Object Detected at sensor 1.");
  }

  if (sensorValue2 == LOW) {
    Serial.println("Object Detected at sensor 2!");
  } else {
    Serial.println("No Object Detected  at sensor 2.");
  }

  if (sensorValue3 == LOW) {
    Serial.println("Object Detected  at sensor 3!");
  } else {
    Serial.println("No Object Detected  at sensor 3.");
  }

  if (sensorValue4 == LOW) {
    Serial.println("Object Detected  at sensor 4!");
  } else {
    Serial.println("No Object Detected  at sensor 4.");
  }

  if (sensorValue5 == LOW) {
    Serial.println("Object Detected  at sensor 5!");
  } else {
    Serial.println("No Object Detected  at sensor 5.");
  }

  if (sensorValue6 == LOW) {
    Serial.println("Object Detected  at sensor 6!");
  } else {
    Serial.println("No Object Detected  at sensor 6.");
  }

  delay(500); // Wait for half a second
}