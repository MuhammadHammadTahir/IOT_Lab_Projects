#define LED_PIN 2


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, HIGH);        // turn on led
  Serial.print("led on");
  delay(1000);                       //wait for 1 second
  digitalWrite(LED_PIN, LOW);       //turn of the led
  delay(1000);                      // wait for 1 second
}
