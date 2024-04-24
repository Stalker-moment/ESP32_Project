#include <WiFi.h>

#define POTENSIO_PIN 34
#define LED_PIN 14
#define WIFI_SSID "p"
#define WIFI_PASSWORD "malah nanya"
#define THINKSPEAK_API_KEY "R2CJGAQVGHXJV30X"

const char *host = "api.thingspeak.com";

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED_PIN, OUTPUT);
  pinMode(POTENSIO_PIN, INPUT);

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

bool sendDataToThingSpeak(float potValue, float pwmValue){
  WiFiClient client;
  if(!client.connect(host, 80)){
    Serial.println("Connection to ThinkSpeak failed!");
    return false;
  }

  String url = "/update?api_key=";
  url += THINKSPEAK_API_KEY;
  url += "&field1=";
  url += String(potValue);
  url += "&field2=";
  url += String(pwmValue);

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
  int potValue = analogRead(POTENSIO_PIN);
  int pwmValue = map(potValue, 0, 4095, 0, 255);

  analogWrite(LED_PIN, pwmValue);

  Serial.print("Potensiometer Value: ");
  Serial.print(potValue);
  Serial.print("\tPWM Value: ");
  Serial.print(pwmValue);

  if(sendDataToThingSpeak(potValue, pwmValue)){
    Serial.println("Data sent to ThingSpeak!");
  } else {
    Serial.println("Failed to send data to ThingSpeak!");
  }

  delay(20000);
}
