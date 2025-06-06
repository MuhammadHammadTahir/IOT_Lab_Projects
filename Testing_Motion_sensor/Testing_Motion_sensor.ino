
// Set GPIOs for PIR Motion Sensor
const int motionSensor = 27;


volatile boolean motion = false;

void IRAM_ATTR handleMotionChange() {
  int state = digitalRead(motionSensor);
  motion = (state == HIGH);  // motion = true if HIGH, false if LOW
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // PIR Motion Sensor mode INPUT
  pinMode(motionSensor, INPUT);
  // Set motionSensor pin as interrupt, assign interrupt function and set CHANGE mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), handleMotionChange, CHANGE);
}

void loop() {
  if(motion == true) {
    Serial.println("MOTION DETECTED!!!");
  }
  if(motion == false) {
    Serial.println("Motion stopped...");
  }
  delay(1000);
}