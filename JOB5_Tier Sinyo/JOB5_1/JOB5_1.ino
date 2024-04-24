#include <WiFi.h>

#define LED 14
const char* ssid = "p";
const char* password = "malah nanya";

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if(client){
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()){
      if (client.available()){
        char c = client.read();
        Serial.write(c);
        if(c == '\n'){
          if(currentLine.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.print("Klik <a href=\"/H\">disini<a/> untuk menyalakan LED. <br>");
            client.print("Klik <a href=\"/L\">disini<a/> untuk mematikan LED. <br>");
            client.println();
            break;
        } else {
          currentLine = "";
        }
      } else if(c != '\r'){
        currentLine += c;
      }

      if (currentLine.endsWith("GEt /h")){
        digitalWrite(LED, HIGH);
      }
      if (currentLine.endsWith("GET /L")){
        digitalWrite(LED, LOW);
      }
    }
  }

  client.stop();
  Serial.println("Client Disconnected");
  }
}

