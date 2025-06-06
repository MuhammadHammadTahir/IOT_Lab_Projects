#include <WiFi.h>
#include <ElegantOTA.h>
#include <HTTPClient.h>  // Use <HTTPClient.h> for ESP32
#include <HTTPUpdate.h>  // Use <HTTPUpdate.h> for ESP32
const char* ssid = "Galaxy A32 035F";
const char* password = "gnty2298";
const char* firmwareUrl = "http://192.168.223.218:8001/lab_08_b.ino.bin";  // firmware URL
WebServer server(80);
WiFiClient client;  // Create WiFiClient instance for HTTPClient
#define LED_BUILTIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected. IP: " + WiFi.localIP().toString());
  ElegantOTA.begin(&server);
  server.on("/trigger-update", []() {
    server.send(200, "text/plain", "Update triggered.");
    // Perform automated OTA update
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(client, firmwareUrl);
      t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl);

      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("Update Failed: %s\n", httpUpdate.getLastErrorString().c_str());
          server.send(500, "text/plain", "Update Failed");
          break;
        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("No Updates Available");
          server.send(304, "text/plain", "No Updates");
          break;
        case HTTP_UPDATE_OK:
          Serial.println("Update Success");
          server.send(200, "text/plain", "Update Success");
          break;
      }
      http.end();
    } else {
      Serial.println("Wi-Fi Disconnected");
      server.send(503, "text/plain", "Wi-Fi Disconnected");
    }
  });
  server.begin();
}
// Non-blocking LED blink
static unsigned long previousMillis = 0;  // Store last time LED was updated
const long interval = 1000;               // Interval for 1-second blink (on/off)
static bool ledState = false;             // Track LED state (HIGH/LOW)
void loop() {
  server.handleClient();
  ElegantOTA.loop();
  unsigned long currentMillis = millis();  // Get current time
  // Check if it's time to toggle the LED
  if (currentMillis - previousMillis >= interval) {
    ledState = !ledState;                              // Toggle state
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);  // Update LED
    previousMillis = currentMillis;                    // Save the current time
  }
}