#include "arduino_secrets.h"
#include "thingProperties.h"

//first steup
#define RED_PIN1   25
#define GREEN_PIN1 26
#define BLUE_PIN1  27
#define TRIG_PIN1  18
#define ECHO_PIN1  19

void setup() {

  //first setup
  pinMode(RED_PIN1, OUTPUT);
  pinMode(GREEN_PIN1, OUTPUT);
  pinMode(BLUE_PIN1, OUTPUT);

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);

  Serial.begin(9600);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  distance1 = measureDistance(TRIG_PIN1, ECHO_PIN1);
  isClose1 = (distance1 <= 30.0);
  onDistance1Change();
  delay(1000);
}

// Mesafe ölçme fonksiyonu
float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2.0;
}



void onDistanceChange(bool close, int id) {
  uint8_t r, g, b;

  if (close) {
    r = 255; g = 0; b = 0;  // Kırmızı
  } else {
    r = 0; g = 255; b = 0;  // Yeşil
  }

  int redPin, greenPin, bluePin;

  // ID'ye göre pin setini seç
  switch (id) {
    case 1:
      redPin = RED_PIN1;
      greenPin = GREEN_PIN1;
      bluePin = BLUE_PIN1;
      break;
    case 2:
      /* redPin = RED_PIN2;
      greenPin = GREEN_PIN2;
      bluePin = BLUE_PIN2;
      break; */
    case 3:
      /* redPin = RED_PIN3;
      greenPin = GREEN_PIN3;
      bluePin = BLUE_PIN3;
      break; */
    default:
      return; // Geçersiz id, hiçbir şey yapma
  }

  // RGB LED'leri kontrol et
  analogWrite(redPin, r);
  delay(10);
  analogWrite(greenPin, g);
  delay(10);
  analogWrite(bluePin, b);

  // Debug
  Serial.print("ID "); Serial.print(id);
  Serial.print(" - R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.println(b);
}
/*
  Since Distance1 is READ_WRITE variable, onDistance1Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onDistance1Change() {
  onDistanceChange(isClose1, 1);
}