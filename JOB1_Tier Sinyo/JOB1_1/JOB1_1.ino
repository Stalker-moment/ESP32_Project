#include <WiFi.h>
#include <WifiClient.h>
#define BLYNK_TEMPLATE_ID "TMPL6loBsuyT2"
#define BLYNK_TEMPLATE_NAME "aoe8w"

#include <BlynkSimpleEsp32.h>

char ssid[] = "p";
char pass[] = "pake nanya";

char auth[] = "cLrYmvTYv5E3xJHDX1eI1Vte6n8aTht5";

#define LED_PIN 2

BlynkTimer timer;

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

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

}

BLYNK_WRITE(V1){
  int pinValue = param.asInt();

  if(pinValue == 1){
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
  }
}
