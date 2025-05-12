#include "arduino_secrets.h"
#include "thingProperties.h"

#define RED_PIN   25
#define GREEN_PIN 26
#define BLUE_PIN  27
#define TRIG_PIN  18
#define ECHO_PIN  19

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  readUltrasonicDistance();
  onDistanceChange();
  delay(1000);
}

void readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float cm = duration * 0.034 / 2;

  distance = cm;
  isClose = (cm <= 30.0);

  Serial.print("Mesafe: ");
  Serial.print(cm);
  Serial.println(" cm");

  
}



void onDistanceChange() {
  uint8_t r, g, b;

  if (distance<=30) {
    

    r = 255;  // Kırmızı
    g = 0;
    b = 0;
  } else {
    

    r = 0;  // Yeşil
    g = 255;
    b = 0;
  }


  // RGB LEDs
  analogWrite(RED_PIN, r);
  delay(10);
  analogWrite(GREEN_PIN, g);
  delay(10);
  analogWrite(BLUE_PIN, b);

  
}