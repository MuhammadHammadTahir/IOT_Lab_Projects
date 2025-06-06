#include <WiFi.h>
#include <ArduinoOTA.h>
const char* ssid = "Galaxy A32 035F";
const char* password = "gnty2298";
#define LED_BUILTIN 2


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected. IP: " + WiFi.localIP().toString());
  ArduinoOTA.setHostname("ESP-OTA");
  ArduinoOTA.begin();
}
void loop() {
  ArduinoOTA.handle();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);  // Blink every 1 second (v1)
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}