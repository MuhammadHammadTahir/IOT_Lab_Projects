#include <WiFi.h>
#include <ElegantOTA.h>
const char* ssid = "Galaxy A32 035F";
const char* password = "gnty2298";
WebServer server(80);
// Non-blocking LED blink
static unsigned long previousMillis = 0;  // Store last time LED was updated
const long interval = 1000;               // Interval for 1-second blink (on/off)
static bool ledState = false;             // Track LED state (HIGH/LOW)

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
  server.begin();
}
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