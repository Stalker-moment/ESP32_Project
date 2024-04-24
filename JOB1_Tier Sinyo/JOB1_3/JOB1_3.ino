#include <WiFi.h>
#include <WifiClient.h>
#define BLYNK_TEMPLATE_ID "TMPL6loBsuyT2"
#define BLYNK_TEMPLATE_NAME "aoe8w"

#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "p";
char pass[] = "pake nanya";
char auth[] = "cLrYmvTYv5E3xJHDX1eI1Vte6n8aTht5";

#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.println(" C");
  Serial.print("Kelembapan: ");
  Serial.print(h);
  Serial.println(" %");

  Blynk.virtualWrite(V3, t);
  Blynk.virtualWrite(V4, h);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();

}
