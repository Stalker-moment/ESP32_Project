#include <WiFi.h>
#include <WifiClient.h>
#define BLYNK_TEMPLATE_ID "TMPL6loBsuyT2"
#define BLYNK_TEMPLATE_NAME "aoe8w"

#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char ssid[] = "p";
char pass[] = "pake nanya";

char auth[] = "cLrYmvTYv5E3xJHDX1eI1Vte6n8aTht5";

#define SERVO_PIN 14
#define VIRTUAL_PIN V0

BlynkTimer timer;
Servo servo;

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

  servo.attach(SERVO_PIN);
  Blynk.virtualWrite(VIRTUAL_PIN, 90);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

}

BLYNK_WRITE(VIRTUAL_PIN){
  int angle = param.asInt();
  servo.write(angle);

  Serial.print("Nilai Sudut : ");
  Serial.println(angle);
}
