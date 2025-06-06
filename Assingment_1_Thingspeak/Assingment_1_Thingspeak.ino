#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "Galaxy A32 035F";  // your network SSID (name)
const char* password = "gnty2298";     // your network password

WiFiClient client;

unsigned long myChannelNumber = 2916959;
const char* myWriteAPIKey = "1D8QS20CWAS0CC7T";

// Set GPIOs for PIR Motion Sensor
const int motionSensor = 27;


volatile boolean motion = false;

int data;

void IRAM_ATTR handleMotionChange() {
  int state = digitalRead(motionSensor);
  motion = (state == HIGH);  // motion = true if HIGH, false if LOW
}

void setup() {
  Serial.begin(115200);  //Initialize serial

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);  // Initialize ThingSpeak

  // PIR Motion Sensor mode INPUT
  pinMode(motionSensor, INPUT);
  // Set motionSensor pin as interrupt, assign interrupt function and set CHANGE mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), handleMotionChange, CHANGE);
}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  if (motion == true) {
    Serial.println("MOTION DETECTED!!!");
    data = digitalRead(motionSensor);
  }
  if (motion == false) {
    Serial.println("Motion stopped...");
    data = digitalRead(motionSensor);
  }



  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, data, myWriteAPIKey);


  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(5000);
}