#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <pgmspace.h>

#define TIME_ZONE -5

#define LED_PIN 2 // GPIO2 for the built-in LED on ESP8266

unsigned long lastMillis = 0;
#define AWS_IOT_STATE_TOPIC "esp8266/state"
#define AWS_IOT_CONTROL_TOPIC "esp8266/control"

// Wi-Fi and AWS IoT credentials
const char WIFI_SSID[] = "eman's S24 Ultra"; // Replace with your Wi-Fi SSID
const char WIFI_PASSWORD[] = "12345678";     // Replace with your Wi-Fi password

#define THINGNAME "ESP8266"

const char MQTT_HOST[] = "a1cpvpt3ls1ke-ats.iot.eu-north-1.amazonaws.com";

#define MQTT_PORT 8883
#define PUBLISH_INTERVAL 5000

static const char cacert[] PROGMEM = R"EOF(
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

static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUSR7Qavws/No1cr3y0aCgf0uS75UwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MDQxMzExMjgw
NFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKlcHYAyq5VOmnam8gKH
VcArlvVQlEBLmLduvNo4Ecy29FTr1ETjcQqoqjMobJcIdtS2ccTdMLFjPykhJ7bb
FjPtVrbCsFX1kb2lkrsQCS3Dn6x7b3E+qKmSMWuF1FFogoh1nqigDLKX0n24CSyP
jRZppRqz38sSe94oNpK6GsStOqEEWnI2LigkcdmQq9ere0Ko2B6d3hnQ/pdkFyP1
Evt0CFfE4QyDBhIH7XMjq2uoI5T/RMRgB9uycNR6EPzf9hJjyFdHVd9wLTSflddy
bgrngHSFR+n3JFPubMEMAH/n0+sYuF1BupRgsGjhj8k7fidT97e8E53doXY4M+ld
tBUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUcCQ5GNQfG3LfUxfJ1OpVcq8rcjswHQYD
VR0OBBYEFEsIggeF87npHJ70erYiI3mfjStoMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQA/cCQFV2cPrwZaQrP5vwom+yEm
zLrqBni12uElQ7gdu7jhlrwY2vK0G7mEtsaVqk1RIgUB++yfUvuloZnAJG0Hu6MQ
Xnx2n7blR7krYaqObiQddJOkOVxLz6/GVqwHItR9yztELIxB9FX21JbNkEuXB0QK
gY4p1pK673RiD6303oQd/xWRC664pZHGer6f47YXqBFNcUf9JrtROj58U9EPE6ku
UuL1cxcBaQN9J/HvfG1hkYW3rpc/HcTRtwAdfC1hE5YzX5RqQzeX0EnLv+h9DgWB
2N/yN4oJ61H4Jm/lYCwh+/M9zJQQoPQ0TPjb5MEqSt9KkgdpIYqeoPbSxOc2
-----END CERTIFICATE-----

)KEY";

static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAqVwdgDKrlU6adqbyAodVwCuW9VCUQEuYt2682jgRzLb0VOvU
RONxCqiqMyhslwh21LZxxN0wsWM/KSEnttsWM+1WtsKwVfWRvaWSuxAJLcOfrHtv
cT6oqZIxa4XUUWiCiHWeqKAMspfSfbgJLI+NFmmlGrPfyxJ73ig2kroaxK06oQRa
cjYuKCRx2ZCr16t7QqjYHp3eGdD+l2QXI/US+3QIV8ThDIMGEgftcyOra6gjlP9E
xGAH27Jw1HoQ/N/2EmPIV0dV33AtNJ+V13JuCueAdIVH6fckU+5swQwAf+fT6xi4
XUG6lGCwaOGPyTt+J1P3t7wTnd2hdjgz6V20FQIDAQABAoIBAQCXkqecerzx7d10
X8ms1a67RMfgWfO073OIiAVhUFkt3HgkIMRjuyMKpRVen8lACwU/LuH3jPbta6O1
jcTbdI0UgE/hVbE33YZIMw11ec6dNL2NQxPkSG3vBbZRQv9YtK5OruYGiOeUVyi9
953OUdO1wWtQ0Ld0KeDOXTMqcoFOjz21dAAKySE1SZIxK990aUvHZU/2QV1Rn5gD
QWcJtSM1EjMGblJaI1YbOHQH/eV/BjeJF2XlOWOiFvapgaUrO3TJ8eN61tYAjYca
BLmVhlYuniu1O1qnoPXN2cGlefjDmYli3aqVLxxw4O9p3EjEtxYHxVBQcQN6CsMO
KJv/SzopAoGBAN4BA3CLfPRb+8jnNbsjdOfaNSUHc3JMTJQ9eIA41S+3Imf5fQJI
JBn7xFgBd3P9HPwquh9d6FSqYQe60jADY194nuuhntGPbeFzj6rxgjqwtk0aUFtc
67Ok82Mbk7/s5BMIu1hfiEoa3/c1mgpSMcrPKkx5mdn+cizeVWwDIYJfAoGBAMNL
WXbmYMO3Y4XepP7ldbaFu8duaqS0MKJzKKhNVeZY7BjhWhWi9Pmxe8JYSeS+xkYr
w+gXuZe3LVIobtJ0QnF6aTDoarynNx4YCk50AXNgY2sDGAW0P79s+zte+YevEaDm
Gt6zKUdpp0Umw1m1yKFS27nUiAHmYMZfbVjJTSYLAoGBALvpnbC40SbBaUkvtAJY
S88om41hVi3sOXHM6l+WO5Vs7TmXK2IwECbSPerEKwlJuSwVOn/sbAimQwVY9Crh
405feh03K4h0YGzR9UybqkDlbRmsaVI8P+JKgB1mrVRqPs+Bt5phGqqnPuYwP9f2
pODnJAWhdEHaJ2ilNSb9K3L/AoGBAKTsT/wiJ5uAlpJp6Gq9+n+ORnrv2y2GYf/+
QlqHHMCvXYrRXFMjT8BByyLfjl+XYDfII+QvjPL7pVFZheVCew378zNzQxhYGirW
4HVKcCMwRiUL8tB2XNNaxHtqBE46YSfAyvZA1N5S8ghq6NQsuObL43T2b3kzRYyg
7+Xn8bfrAoGAHDiHiP5CdiOZl1rokvPc6FbA7OgWWiH/AYjhOqQxgxF3Re0efOah
OZo5t+jt/ToqCXUqwFVozbbo4YpWOBkvtnlwLaaUBpxdaZRBbKpISDJKGMBz+2Lc
UX+cmCUKMMpjZc3eK76e4L+XwaYKnq/HoOho9zpvInDDNhtqoP6eyKI=
-----END RSA PRIVATE KEY-----

)KEY";

WiFiClientSecure net;
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
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

    NTPConnect();

    net.setTrustAnchors(&cert);
    net.setClientRSACert(&client_crt, &key);

    client.setServer(MQTT_HOST, 8883);
    client.setCallback(messageReceived);

    Serial.println("Connecting to AWS IoT");

    while (!client.connect(THINGNAME)) {
        Serial.print(".");
        delay(1000);
    }

    if (!client.connected()) {
        Serial.println("AWS IoT Timeout!");
        return;
    }
    
    client.subscribe(AWS_IOT_CONTROL_TOPIC);
    Serial.println("AWS IoT Connected!");
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
