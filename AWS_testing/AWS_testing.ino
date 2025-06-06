#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <pgmspace.h>

#define TIME_ZONE -5

#define LED_PIN 27 // GPIO2 for the built-in LED on ESP8266

unsigned long lastMillis = 0;
#define AWS_IOT_STATE_TOPIC "esp32/state"
#define AWS_IOT_CONTROL_TOPIC "esp32/control"

// Wi-Fi and AWS IoT credentials
const char WIFI_SSID[] = "Galaxy A32 035F"; // Replace with your Wi-Fi SSID
const char WIFI_PASSWORD[] = "gnty2298";     // Replace with your Wi-Fi password

#define THINGNAME "ESP32"

const char MQTT_HOST[] = "a1bombpawtx3xj-ats.iot.ap-south-1.amazonaws.com";

#define MQTT_PORT 8883
#define PUBLISH_INTERVAL 5000

//static const char cacert[] PROGMEM = R"EOF(
const char* cacert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

//static const char client_cert[] PROGMEM = R"KEY(
const char* client_cert = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAOH8fjlfSVM4NZTjB2CtKaVtQ0zhMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNTA0MTUwMzQy
MjVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDW5pvLAmD9g8lkNSPz
sPeVHiqDspXjejcqo7wXaWNukhAOobMQ5d+5GF4dccqZJYN5htrLxEUcIrqypViN
ijo3cXLXmM/M6nvTuF/EwM+tzaW8VarQTWX8sE/vPu7YE8T1/Sf51C4di/ZkA9OG
qGZNZvNMdBb/HRmRA4VOGgnRBZKgAkM+zlMM/yNS5b56oIqIMKjntnGuwQz5P29J
7uy7BkByOaOFblIuelTELSkDtFsP90rogyaK7R/wMqqfcCHBSpP9xL6E7+DZNgrm
xfYD640Py9Iyf2Rp0pADnchYsWrwD/fHML7pFnUSDE9eO6HkqkDjUJ8wr+ZFegk9
sjmFAgMBAAGjYDBeMB8GA1UdIwQYMBaAFBQeipaH76pHPAIjeXL/Y6hLqa/eMB0G
A1UdDgQWBBR9jEhLw2L1NaH79cJvuIU7e0+9RDAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAreHYJ7/dzx5lUo7hipNXXRN3
sSGogc16ZZInO/bUli2ayb5A9RJC3uV/yqKyzXCtBL/AgAt6EGCtr/Gx1hPup+P7
9roW96PrdsvxUKiNlF/uAzjIforu7gaoYE05vbm3cOMwitJWW/Q8BnsXIesVp3zO
2yDGkAp+/NsnqPhDLdcCa7DazwhXR42WmDm3JjOYbRznZGAlmCC8rX1XNUK8BQlB
isfT7lpRy68OPsE8/ps/eBuvSY0WWdjuVL3n+mFzT5COJ/cB7jozttPJ5d3xz6U1
ItIVa46voJF4MbxqpSCcb4vvAKxibrh7MRCl5Qe49+MMF1eLxbmfbeiScSZNrQ==
-----END CERTIFICATE-----

)KEY";

//static const char privkey[] PROGMEM = R"KEY(
const char* privkey = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA1uabywJg/YPJZDUj87D3lR4qg7KV43o3KqO8F2ljbpIQDqGz
EOXfuRheHXHKmSWDeYbay8RFHCK6sqVYjYo6N3Fy15jPzOp707hfxMDPrc2lvFWq
0E1l/LBP7z7u2BPE9f0n+dQuHYv2ZAPThqhmTWbzTHQW/x0ZkQOFThoJ0QWSoAJD
Ps5TDP8jUuW+eqCKiDCo57ZxrsEM+T9vSe7suwZAcjmjhW5SLnpUxC0pA7RbD/dK
6IMmiu0f8DKqn3AhwUqT/cS+hO/g2TYK5sX2A+uND8vSMn9kadKQA53IWLFq8A/3
xzC+6RZ1EgxPXjuh5KpA41CfMK/mRXoJPbI5hQIDAQABAoIBAQCJyJOMxrqu8GcY
U6X4YbdrqUqy47aMgdiLAy3eiNiSlR2sc+peZDRb9Y6qLEIsjMvOCDOSQAXbczzm
u3KaVUzcLWXna3eYN6Mq7OSiPsko81hW90osgeNo7xZhG6WBbrNy0FxoJq1mjk1s
dLOTe7cT23UUyL3R1bOFsY8vsywmEdLaFwG5zlCS1DRDf6i1mzlNgXMz0ZuX+88/
hQrEPH+P/hi0ZjzuF8vTsnpJdFgplalYn5oigRlkyTv19WZiT/HbPNN6P53hwsro
PKuTFlfUSDBB9oV2It3MmUHxE4zk8VQtMfRBEYUtlFMnrxLk1hOJtc81nOZUATEs
XntDdTFhAoGBAOtfZg8lBCwVM4V+i/iRbTzwM+/Qbx8a12Sd/Lx8pOGOCNRaHEmi
hXyCplzofJ2Zn5+PsiOgPuHm87nfbGO1jQi53gIXtpT29m+L7OLL3YZ5vHizxjOK
rXZygSh4DtSf/ox0lIfTUA9FBSqhgS+olXTXWTfDqD7Yt7LX/EvZfJRNAoGBAOm7
7ED2jnx5nYa9pPvHb6+tn4QSkG4KAca/964SjAU23KDVc3jxnlFoEMiQCRIYYDNO
15Z1DqL6KrTpyulKzeanvlz8KISBx3rr94+qpfxKY7Df2KkmHX0pM/AcuJHo5qw3
V9LzAgEfAsF36GoeGttfFOWxfJibuIoaHh7b/bYZAoGAUy2La56xMsLrSinGXTlH
WVmpgP4vxtY9eBk8z39c5TjEdgbCKGN+uNmS4RJnt2oqtsCZO23NyxdK2kzSiQoH
pSXKBwNKSqpbvF++fwd8aaFrsVwWlvBTDKC6dAq5cvp3gjtQog9TI5JImSAp/yWc
a4YOyV4jftInWNRrHILnO6ECgYEA0WAQ46S5wNFkhjXN2+ksqC+czD+g96GpM7P1
GA7vNSCPBNgQp4+xZXUWS8OTDI2fZ3bB8rqLUP1OUl938AGdbnhnM36CgSiMXBWS
3zRbmiO9J81tlSBanCrTpszjnXkHYc/1sTcUXZK88jgKjzzGjCqhWpziYwZjifqZ
bsVcgckCgYEAsIeUzokKfxBKqa9uP+gdMCHn6AUdIJsmiczfZCtinIlVtIpRNUkz
eX57u26AxoSrF99AIyaGdoCHm2ieBdzhhiga/ObjeUxjUzEikELSBsIZMsqAMsOQ
mq1Wp75Faaj2YNl9dhLlp36xrrsPjmo/BqwI1baMeq5GBUOepqRSjIo=
-----END RSA PRIVATE KEY-----

)KEY";

WiFiClientSecure net;

/*WiFiClientSecure net;
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);*/
PubSubClient client(net);

time_t now;
time_t nowish = 1510592825;

void NTPConnect() {
    Serial.print("Setting time using SNTP");
    configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
    now = time(nullptr);
    while (now < nowish) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("done!");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

void messageReceived(char *topic, byte *payload, unsigned int length) {
    Serial.print("Received [");
    Serial.print(topic);
    Serial.print("]: ");
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);

    if (message == "ON") {
        digitalWrite(LED_PIN, HIGH);
        publishLEDState(true);
    } else if (message == "OFF") {
        digitalWrite(LED_PIN, LOW);
        publishLEDState(false);
    }
}

void publishLEDState(bool state) {
    if (!client.connected()) {
        Serial.println("Cannot publish - not connected");
        return;
    }

    StaticJsonDocument<200> doc;
    doc["device"] = THINGNAME;
    doc["led_state"] = state ? "ON" : "OFF";
    doc["timestamp"] = time(nullptr);
    
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);
    
    Serial.print("Publishing to ");
    Serial.print(AWS_IOT_STATE_TOPIC);
    Serial.print(": ");
    Serial.println(jsonBuffer);

    if (client.publish(AWS_IOT_STATE_TOPIC, jsonBuffer)) {
        Serial.println("Publish succeeded");
    } else {
        Serial.println("Publish failed!");
        Serial.println(client.state()); // Print MQTT connection state
    }
}

// Add the missing connectAWS function
void connectAWS() {
    delay(3000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nWiFi connected");

    NTPConnect();

      net.setCACert(cacert);
      net.setCertificate(client_cert);
      net.setPrivateKey(privkey);

    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(messageReceived);

    while (!client.connected()) {
        Serial.print("Connecting to AWS IoT MQTT... ");
        if (client.connect(THINGNAME)) {
            Serial.println("connected!");

            // Subscribe to the control topic
            client.subscribe(AWS_IOT_CONTROL_TOPIC);
            Serial.print("Subscribed to topic: ");
            Serial.println(AWS_IOT_CONTROL_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    connectAWS();
    publishLEDState(true);
}

void loop() {
    static unsigned long lastPublish = 0;
    
    if (!client.connected()) {
        Serial.println("Disconnected - Attempting reconnect");
        connectAWS();
    }
    
    client.loop();  // Maintain MQTT connection
    
    // Regular state publishing
    if (millis() - lastPublish > PUBLISH_INTERVAL) {
        lastPublish = millis();
        bool currentState = (digitalRead(LED_PIN) == HIGH); // LOW = ON for ESP8266
        publishLEDState(currentState);
        
        // Debug connection status
        Serial.print("MQTT Connected: ");
        Serial.println(client.connected() ? "YES" : "NO");
        Serial.print("WiFi Connected: ");
        Serial.println(WiFi.status() == WL_CONNECTED ? "YES" : "NO");
    }
}
