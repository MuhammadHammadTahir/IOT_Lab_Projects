#define RELAY_PIN D6

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Relay Controlled Initialized");

}

void loop() {

  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Relay ON");
  delay(5000);

  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Relay OFF");
  delay(5000);
}
