

#include <Servo.h>
#include <Adafruit_Fingerprint.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const Servo moters[4] = {Servo(), Servo(), Servo(), Servo()};

const int MOTERS_CODE[4] = {1, 2, 3, 4};

uint8_t enrollFingerprint();

uint8_t openTheDoor();

void setup() {
  
  // 서보모터 등록
  moter[0].attach(MOTERS_CODE[0]);
  moter[1].attach(MOTERS_CODE[1]);
  moter[2].attach(MOTERS_CODE[2]);
  moter[3].attach(MOTERS_CODE[3]);
  
  // 지문 등록 센서
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  finger.getParameters();
  
  // 푸시버튼
  pinMode(8, INPUT); //지문 등록 버튼
  pinMode(9, INPUT); //지문 사용
}

void loop() {
  if(digitalRead(8) == HIGH){
    delay(1000);
    while(digitalRead(8) != HIGH){
      if(enrollFingerprint())
        break;
    }
  }else if(digitalRead(9) == HIGH){
    delay(1000);
    while(digitalRead(9) != HIGH){
      int id;
      if(id = openTheDoor()){
        
        break;
      }
    }
  }
}

uint8_t enrollFingerprint(){
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  
  // OK success!
  
  p = finger.image2Tz(1);
  if(p != FINGERPRINT_OK){
    return p;
  }
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  // OK success!
  p = finger.image2Tz(2);
  if(p != FINGERPRINT_OK){
    return p;
  }
  p = finger.createModel();
  if (p != FINGERPRINT_OK) {
    return p;
  }
  p = finger.storeModel(id);
  if (p != FINGERPRINT_OK) {
    return p;
  }

  return true;
}

uint8_t openTheDoor(){
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK){
    return p;
  }

  // OK success!

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){
    return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) {
    return p;
  } 

  return finger.fingerID;
}
