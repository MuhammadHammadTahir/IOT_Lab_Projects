#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi Credentials (same as publisher)
const char* ssid = "IoT-Class";
const char* password = "iotclass1";

// MQTT Broker
const char* mqtt_server = "test.mosquitto.org";  // Check Broker
const int mqtt_port = 1883;
const char* topic = "pieas/iotlab/4/group/4/task3/button";  // Match publisher
WiFiClient espClient;
PubSubClient client(espClient);

// Button Setup
const int buttonPin = D6;
bool lastButtonState = LOW;

// LED Setup
const int ledPin = D7;
void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // Initially LED is OFF
  setupWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);  // Handle incoming messages
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
    if (client.connect("p.iot.0.569")) {  // Use a random unique device id.
      Serial.println("MQTT Connected");
      client.subscribe(topic);  // Additional Line to subscribe topic
    } else {
      Serial.println("Trying to connect MQTT ... ");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  //Print received message on Serial Console
  Serial.print("Got Message: ");
  Serial.print(topic);
  Serial.print(" > ");
  Serial.println(message);

  //Set LED state based on the received Message
  if (message == "ON") {
    digitalWrite(ledPin, LOW);
  } else if (message == "OFF") {
    digitalWrite(ledPin, HIGH);
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