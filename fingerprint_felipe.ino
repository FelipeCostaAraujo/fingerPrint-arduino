#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(11, 12);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
const int rs = 2, en = 3 , d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs , en , d4, d5, d6 , d7);

void setup()
{
  lcd.begin(16, 2);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.print("\n\nAdafruit finger detect test");

  lcd.setCursor(0, 0);
  lcd.print("Aguard digital");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");

}

void loop()                     // run over and over again
{
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
  digitalWrite(8, HIGH);
}

uint8_t getFingerprintID() {

  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;


  // found a match!

  Serial.print("Found ID #"); Serial.print(finger.fingerID);

  digitalWrite(12, HIGH);
  digitalWrite(8, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(12, LOW);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);


  getNameFingerPrint(finger.fingerID);

  return finger.fingerID;
}

void getNameFingerPrint(int id) {
  lcd.clear();
  lcd.display();
  lcd.setCursor(0, 0);
  lcd.print("Ultima Digital");
  lcd.setCursor(0, 1);
  switch (id) {
    case 1:
      lcd.print("Felipe");
      break;
    case 2:
      lcd.print("Daniela");
      break;
    case 3:
      lcd.print("Elisabete");
      break;
    case 4:
      lcd.print("Daiane");
      break;
    case 5:
      lcd.print("Cicero");
      break;
    case 10:
      lcd.print("Felipe");
      break;
  }
}
