#define BLYNK_TEMPLATE_ID "TMPL6swm3q_lu"
#define BLYNK_TEMPLATE_NAME "Smart Street Light"
#define BLYNK_AUTH_TOKEN "DY27yZvRw2DJUZ-EZ5TQ_Y3_jOaQrVv0"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Galaxy A32 035F";
char pass[] = "gnty2298";

#define DHTPIN 25      // GPIO where DHT11 is connected
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0){
  int value = param.asInt();
  if(value == 1){
    digitalWrite(26, HIGH);
     Serial.println("V0: 1");
  }else{
   digitalWrite(26, LOW);
    Serial.println("V0: 0");
  }
  }
BLYNK_WRITE(V1){
    int value = param.asInt();
  if(value == 1){
    digitalWrite(27, HIGH);
     Serial.println("V1: 1");
  }else{
   digitalWrite(27, LOW);
    Serial.println("V1: 0");
  }
  }

  void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temp: "); Serial.print(t); Serial.print(" °C  ");
  Serial.print("Humidity: "); Serial.print(h); Serial.println(" %");

  Blynk.virtualWrite(V2, t);  // Send temp to Blynk
  Blynk.virtualWrite(V3, h);  // Send humidity to Blynk
}

void setup(){
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // Read every 5 seconds
  timer.setInterval(5000L, sendSensor);
}
void loop(){
  Blynk.run();
  timer.run();
}