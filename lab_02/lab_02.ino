#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN D5           //  Data pin connected to the sensor (GPIO2)
#define DHTTYPE DHT22       //  As we are using DHT22 sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("DHT Sensor Reading");
  dht.begin();
}

void loop() {
  // wait for 2 seconds between measurments.
  delay(2000);

  // Reading Temperature and Humidity  values.
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();  //  Temperature is in Celsius

  //  Check if any read failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)){
    Serial.println("Failed to read form DHT sensor!");
    return;
  }

  //  Print the readings to the serial Monitor.
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C");
  Serial.print("\n");
}
