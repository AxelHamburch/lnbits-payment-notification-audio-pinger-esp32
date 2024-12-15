#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "music.h"

const int piezoPin = 2;

extern const char *ssid;
extern const char *password;

// LNbits and SSE configuration
extern const char *lnbitsHost;
extern const char *lnbitsReadApiKey;
String sseEndpoint = String(lnbitsHost) + "/api/v1/payments/sse?api-key=" + lnbitsReadApiKey;

// Function to connect to Wi-Fi
void connectToWiFi() {
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// Function to listen to the SSE endpoint
void listenToSSE() {
    HTTPClient http;

    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Wi-Fi lost. Reconnecting...");
            connectToWiFi();
        }

        Serial.println("Connecting to SSE endpoint...");
        http.begin(sseEndpoint);
        http.addHeader("Connection", "keep-alive");
        http.addHeader("Cache-Control", "no-cache");
        http.addHeader("Accept", "text/event-stream");
        http.setTimeout(30000); // 30 seconds

        int httpCode = http.GET();
        Serial.printf("HTTP response code: %d\n", httpCode);

        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                WiFiClient *stream = http.getStreamPtr();
                while (http.connected()) {
                    while (stream->available()) {
                        String line = stream->readStringUntil('\n');
                        if (line.startsWith("data:")) {
                            line = line.substring(5); // Remove "data:" prefix
                            StaticJsonDocument<512> doc;
                            DeserializationError error = deserializeJson(doc, line);

                            if (!error) {
                                bool pending = doc["pending"] | true; // Default to true if not present
                                if (!pending) {
                                    Serial.println("Received successful payment.");
                                    playMusic("E1E1E2E1E1E2E1G1C1D1E4F1F1F1F1F1E1E1E1E1D1D1E1D2G2");
                                }
                            }
                        }
                    }
                    delay(10);
                }
            }
        } else {
            Serial.printf("HTTP error: %d\n", httpCode);
        }

        http.end();
        delay(5000);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(piezoPin, OUTPUT);

    ledcSetup(0, 500, 13); // Channel 0, 500 Hz, 13-bit resolution
    ledcAttachPin(piezoPin, 0);

    connectToWiFi();

    // Play music at setup
    playMusic("E1E1E2E1G1C1D1");

    // Start listening to SSE
    xTaskCreate([](void *parameter) { listenToSSE(); },
                "SSEListenerTask",
                8192,
                NULL,
                1,
                NULL);
}

void loop() {
    // Do nothing in the main loop
}
