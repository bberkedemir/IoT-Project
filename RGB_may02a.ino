#include "arduino_secrets.h"
#include "thingProperties.h"


#define RED_PIN1   25
#define GREEN_PIN1 26
//#define BLUE_PIN1  27

#define RED_PIN2   32
#define GREEN_PIN2 33

#define RED_PIN3   22
#define GREEN_PIN3 23

#define TRIG_PIN1  18
#define ECHO_PIN1  19

#define TRIG_PIN2  15
#define ECHO_PIN2  2

#define TRIG_PIN3  4
#define ECHO_PIN3  5

#define BUZZER_PIN 21

void setup() {

  
  pinMode(RED_PIN1, OUTPUT);
  pinMode(GREEN_PIN1, OUTPUT);
  //pinMode(BLUE_PIN1, OUTPUT);

  pinMode(RED_PIN2, OUTPUT);
  pinMode(GREEN_PIN2, OUTPUT);

  pinMode(RED_PIN3, OUTPUT);
  pinMode(GREEN_PIN3, OUTPUT);

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);

  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);

  Serial.begin(9600);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  digitalWrite(BUZZER_PIN, HIGH); 
  distance1 = measureDistance(TRIG_PIN1, ECHO_PIN1);
  distance2 = measureDistance(TRIG_PIN2, ECHO_PIN2);
  distance3 = measureDistance(TRIG_PIN3, ECHO_PIN3);
  isClose1 = (distance1 <= 10.0);
  isClose2 = (distance2 <= 10.0);
  isClose3 = (distance3 <= 10.0);
  onDistance1Change();
  onDistance2Change();
  onDistance3Change();
  

  // Buzzer için
  static bool previousAllOccupied = false;
  bool currentAllOccupied = isClose1 && isClose2 && isClose3;

  // Önceden doluydu, şimdi biri boşaldıysa → araba çıktı demek
  if (previousAllOccupied && !currentAllOccupied) {
    carOut = true;      // Araç çıktığını bildir
    onCarOutChange();   // IoT Cloud'a bildir
  }

  if(!previousAllOccupied && currentAllOccupied) {
    carOut = false;      // Araç girdiğini bildir
    onCarOutChange();   // IoT Cloud'a bildir
  }

  previousAllOccupied = currentAllOccupied;  // Önceki durumu güncelle

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
      //bluePin = BLUE_PIN1;
      break;
    case 2:
      redPin = RED_PIN2;
      greenPin = GREEN_PIN2;
      //bluePin = BLUE_PIN2;
      break; 
    case 3:
      redPin = RED_PIN3;
      greenPin = GREEN_PIN3;
      //bluePin = BLUE_PIN3;
      break; 
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
/*
  Since Distance2 is READ_WRITE variable, onDistance2Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onDistance2Change()  {
  onDistanceChange(isClose2, 2);
  // Add your code here to act upon Distance2 change
}
/*
  Since Distance3 is READ_WRITE variable, onDistance3Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onDistance3Change()  {
  onDistanceChange(isClose3, 3);
  // Add your code here to act upon Distance3 change
}
/*
  Since CarOut is READ_WRITE variable, onCarOutChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onCarOutChange()  {
  Serial.print("carOut değişti: ");
  Serial.println(carOut);
  // Buzzer bir kere 1 saniye çalsın
  digitalWrite(BUZZER_PIN, LOW);   // Buzzer'ı kapat
  delay(100);  // 1 saniye bekle
  digitalWrite(BUZZER_PIN, HIGH);  // Buzzer'ı aç
  delay(100);  // 1 saniye boyunca buzzer'ı çaldır
  digitalWrite(BUZZER_PIN, LOW);   // Buzzer'ı kapat
  delay(100);  // 1 saniye bekle
  digitalWrite(BUZZER_PIN, HIGH);  // Buzzer'ı aç
  delay(100);  // 1 saniye boyunca buzzer'ı çaldır
  digitalWrite(BUZZER_PIN, LOW);   // Buzzer'ı kapat
  delay(100);  // 1 saniye bekle
  digitalWrite(BUZZER_PIN, HIGH);  // Buzzer'ı aç
  delay(100);  // 1 saniye boyunca buzzer'ı çaldır
}