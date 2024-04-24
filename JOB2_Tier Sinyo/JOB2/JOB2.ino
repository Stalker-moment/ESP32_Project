#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "p";
const char* password = "malah nanya";

void setup() {
  Serial.begin(9600);
  connectWifi();
}

void loop() {
  ApiGetSystem();
  delay(3000);
}

void connectWifi() {
  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to the WiFi network");
}

void ApiGetSystem() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Lokasi URL API yang akan diminta
    String url = "http://natslock.my.id/api/systemdb/all";

    Serial.print("Sending HTTP GET request to: ");
    Serial.println(url);

    // Menggunakan WiFiClient untuk memulai koneksi
    WiFiClient client;
    http.begin(client, url);

    int httpCode = http.GET();
    Serial.println(httpCode);
    if (httpCode == 502) {
      Serial.print("HTTP request failed with error code: ");
      Serial.println(httpCode);
    }
    if (httpCode > 0) {
      // Penerimaan respon berhasil
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Response payload:");
        Serial.println(payload);

        // Parsing JSON
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        // Mendapatkan nilai OnGoing.stats
      }
    } else {
      Serial.print("HTTP request failed with error code: ");
      Serial.println(httpCode);
    }

    // Menutup koneksi
    http.end();
  }
}