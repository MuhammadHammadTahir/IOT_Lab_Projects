#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Serial.print("Temperature: "); Serial.print(temp);
  Serial.print("C Humidity: ");  Serial.print(hum);
  Serial.println("%");
  delay(2000);
}
