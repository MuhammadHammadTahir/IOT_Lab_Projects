#define LED_PIN D6

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("LED Control Initialized");

}

void loop() {
  
  for(int brightness=0; brightness<=255; brightness++)
  {
    analogWrite(LED_PIN, brightness);
    delay(10);
  }

  for(int brightness=255; brightness>=0; brightness--)
  {
    analogWrite(LED_PIN, brightness);
    delay(10);
  }

}
