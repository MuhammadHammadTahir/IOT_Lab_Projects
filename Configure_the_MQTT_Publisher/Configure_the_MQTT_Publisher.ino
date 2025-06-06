#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Wi-Fi Credentials
const char* ssid = "Galaxy A32 035F";
const char* password = "gnty2298";

//MQTT Broker
const char* mqtt_server ="test.mosquitto.org";
const int mqtt_port = 1883;
const char* topic = "pieas/iotlab/4/group/4/button";

WiFiClient espClient;
PubSubClient client(espClient);

// Button Setup
const int buttonPin = D6;
bool lastButtonState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  setupWiFi();
  client.setServer(mqtt_server, mqtt_port);
}

void setupWiFi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
}

void reconnect() {
 while (!client.connected()) {
 if (client.connect("p.iot.0.567")) { // Use a random unique device id.
 Serial.println("MQTT Connected");
 } else {
 Serial.println("Trying to connect MQTT ... ");
 delay(5000);
 }
 }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  bool buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      Serial.println("Button is ON");
      client.publish(topic, "ON");
    } else {
      Serial.println("Button is OFF");
      client.publish(topic, "OFF");
    }
    lastButtonState = buttonState;
  }
  delay(50);
}
