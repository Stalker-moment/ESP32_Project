#include <WiFi.h>
#include <WifiClient.h>
#define BLYNK_TEMPLATE_ID "TMPL6loBsuyT2"
#define BLYNK_TEMPLATE_NAME "aoe8w"

#include <BlynkSimpleEsp32.h>

char ssid[] = "p";
char pass[] = "pake nanya";

char auth[] = "cLrYmvTYv5E3xJHDX1eI1Vte6n8aTht5";

#define FLAME_SENSOR_PIN 14
#define PIN_LED1 V0
#define PIN_LED2 V1
#define PIN_LABEL V2

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

  pinMode(FLAME_SENSOR_PIN, INPUT);
  timer.setInterval(1000L, checkFlameStatus);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}

void checkFlameStatus(){
  int flameValue = digitalRead(FLAME_SENSOR_PIN);

  if(flameValue == HIGH){
    Blynk.virtualWrite(PIN_LED1, HIGH);
    Blynk.virtualWrite(PIN_LED2, LOW);
    Blynk.virtualWrite(PIN_LABEL, "KONDISI BAHAYA, API TERDETEKSI");

    Serial.print("Nilai Pembacaan Sensor: ");
    Serial.println(flameValue);
    Serial.println("KONDISI BAHAYA, API TERDETEKSI");
    Serial.println("Menghidupkan LED 1 (V0)");
    Serial.println(" ");
  } else {
    Blynk.virtualWrite(PIN_LED1, LOW);
    Blynk.virtualWrite(PIN_LED2, HIGH);
    Blynk.virtualWrite(PIN_LABEL, "KONDISI AMAN, API TIDAK TERDETEKSI");

    Serial.print("Nilai Pembacaan Sensor: ");
    Serial.println(flameValue);
    Serial.println("KONDISI AMAN, API TIDAK TERDETEKSI");
    Serial.println("Menghidupkan LED 2 (V1)");
    Serial.println(" ");
  }
}
