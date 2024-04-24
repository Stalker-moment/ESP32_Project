#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 12
#define DHTTYPE DHT11
#define WIFI_SSID "p"
#define WIFI_PASSWORD "malah nanya"
#define THINKSPEAK_API_KEY "3NF2B56X2UZ48866"

DHT dht(DHTPIN, DHTTYPE);

const char *host = "api.thingspeak.com";

void setup() {
  Serial.begin(115200);
  delay(100);

  dht.begin();

  connectToWiFi();
}

void connectToWiFi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi....");
  }

  Serial.println("Connected to WiFi");
}

bool sendDataToThingSpeak(float temperature, float humidity){
  WiFiClient client;
  if(!client.connect(host, 80)){
    Serial.println("Connection to ThinkSpeak failed!");
    return false;
  }

  String url = "/update?api_key=";
  url += THINKSPEAK_API_KEY;
  url += "&field1=";
  url += String(temperature);
  url += "&field2=";
  url += String(humidity);

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: Close\r\n\r\n");

  delay(1000);

  String response = "";
  while(client.available()){
    char c = client.read();
    response += c;
  }

  Serial.println("Response from ThingSpeak: ");
  Serial.println(response);

  if(response.indexOf("Status: 200 OK") == -1){
    Serial.println("Failed to update ThinkSpeak!");
    return false;
  }

  client.stop();
  return true;
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if(sendDataToThingSpeak(temperature, humidity)){
    Serial.println("Data sent to ThingSpeak!");
  } else {
    Serial.println("Failed to send data to ThingSpeak!");
  }

  delay(20000);
}
