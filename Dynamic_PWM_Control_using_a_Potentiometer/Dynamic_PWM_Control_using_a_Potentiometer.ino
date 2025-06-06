#define pot_pin A0
#define LED_PIN D6

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int pot_val = analogRead(pot_pin);
  Serial.println(pot_val);

  int brightness = map(pot_val, 30, 1023, 0, 255);

  analogWrite(LED_PIN, brightness);

  Serial.print("Brightness: ");
  Serial.println(brightness);
  delay(50);
}
