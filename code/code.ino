#include <SPI.h>  // RFID
#include <MFRC522.h> // RFID Sensor
#include <Wire.h> // I2C, for Thermometre
#include <avr/wdt.h> // Watchdog timer
#include <Adafruit_MLX90614.h> // thermometer
#define SS_PIN 10 // RFID
#define RST_PIN 7 // RFID
#define LOGIC 4
#define LED1 2 // Red
#define LED2 3 // Blue


MFRC522 mfrc522(SS_PIN, RST_PIN); 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(57600) ;
  SPI.begin();
  
  // RFID Setup
  mfrc522.PCD_Init() ;
  Serial.println("Setup 1");

  // RELAY Setup
  pinMode(LOGIC, OUTPUT) ;
  Serial.println("Setup 2");
  
  // Temperature Setup
  const int constant = 1.5 ;// kalibrasi thermometer terhadap jarak pengukuran 5 cm
  mlx.begin();
  Serial.println("Setup 3");
  
  // Indicator Setup
  pinMode(LED1, OUTPUT) ;
  pinMode(LED2, OUTPUT) ;
  Serial.println("Setup 4");
  
  // Watchdog timer setup 
//  wdt_enable(WDTO_8S) ;
  Serial.println("Setup 5");
  
  // Success Setup
  for (int i = 0; i < 3 ; i++ ){
    digitalWrite(LED1, HIGH);
    delay(500) ;
    digitalWrite(LED1, LOW) ;  
  }
  Serial.println("Setup Success");
}

void loop() {
  wdt_reset() ;
//  Serial.println("Begin test") ;
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.println("pass") ;
  String userID = "";
  byte letter ;
  for (byte i = 0; i < mfrc522.uid.size; i++){
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     userID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     userID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  wdt_reset() ;
  Serial.println();
  userID.toUpperCase();
  if (userID.substring(1) == "02 AC 7E D1 E1 0E B0" || userID.substring(1) == "E5 AD 8D 4F") {
    digitalWrite(LED1, HIGH) ;
    Tempcheck() ;   
    delay(2000) ;
  } else {
    delay(2000) ;
  }

  //reset indicator
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  Serial.println("reset complete") ;
  delay(2000);
}

void Tempcheck(){
  wdt_reset() ;
  while (1){
    float temp = mlx.readObjectTempC() ;
    if (temp > 34 && temp < 40) {
      Serial.println(temp) ;
      if (temp >= 37 ){
        digitalWrite(LED1, LOW) ;
        Serial.println("GO AWAY");
        } else {
        Serial.println("GET IN!") ;
        digitalWrite(LED2, HIGH) ;
        digitalWrite(LOGIC, HIGH) ;
        delay (5000) ;
        digitalWrite(LOGIC, LOW) ;
        }
        wdt_reset() ;
        break ;
//      Serial.println("Breaking out");
    } else {
      wdt_reset() ;
      continue ; 
    }
  }


}
