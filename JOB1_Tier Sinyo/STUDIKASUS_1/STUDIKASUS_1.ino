#include <WiFi.h>
#include <WifiClient.h>
#define BLYNK_TEMPLATE_ID "TMPL6loBsuyT2"
#define BLYNK_TEMPLATE_NAME "aoe8w"

#include <BlynkSimpleEsp32.h>

char ssid[] = "p";
char pass[] = "pake nanya";

char auth[] = "cLrYmvTYv5E3xJHDX1eI1Vte6n8aTht5";

#define RED 25
#define GREEN 33
#define BLUE 32
#define LEDAnalog 26
#define echo 14
#define trig 27
#define VirtualLabel V4

BlynkTimer timer;

long duration, distance; // Variables to store duration and distance

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

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(LEDAnalog, OUTPUT);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 

  timer.setInterval(500L, readSensor);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V0){
  int pinValue = param.asInt();

  if(pinValue == 1){
    digitalWrite(RED, HIGH);
    Serial.println("LED RED ON");
  } else {
    digitalWrite(RED, LOW);
    Serial.println("LED RED OFF");
  }
}

BLYNK_WRITE(V1){
  int pinValue = param.asInt();

  if(pinValue == 1){
    digitalWrite(GREEN, HIGH);
    Serial.println("LED GREEN ON");
  } else {
    digitalWrite(GREEN, LOW);
    Serial.println("LED GREEN OFF");
  }
}

BLYNK_WRITE(V2){
  int pinValue = param.asInt();

  if(pinValue == 1){
    digitalWrite(BLUE, HIGH);
    Serial.println("LED BLUE ON");
  } else {
    digitalWrite(BLUE, LOW);
    Serial.println("LED BLUE OFF");
  }
}

BLYNK_WRITE(V3){
  int pinValue = param.asInt();

  analogWrite(LEDAnalog, pinValue);
}

void readSensor() {
  digitalWrite(trig, LOW); 
  delayMicroseconds(2);

  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig, LOW); 

  duration = pulseIn(echo, HIGH); 
  distance = duration * 0.034 / 2;

  if (distance >= 200 || distance <= 0) {
    Serial.println("Out of range");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  Blynk.virtualWrite(VirtualLabel, distance);
}
