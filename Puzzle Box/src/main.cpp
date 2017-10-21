/*
  Escape Box

  Git: https://github.com/retronics/Puzzle-Box/

 */

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>


//Card reader
constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

//Knocking Defines
#define Solenoid 7
#define SolenoidOnDelay 20
#define KnockInDigitDelay 750
#define KnockBetweenDigitDelay 1500

bool DoKnocking = true;

byte Combination[] = { 3, 2, 1, 4}; //Lock combination
byte Mode = 0; //Board mode

///////////////////////////////////////////////////////////
void setup() {

  Serial.begin(9600); //Init serial
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 (card reader)

  Serial.println("Serial started...");

  pinMode(Solenoid, OUTPUT); // initialize the solenoid

}

///////////////////////////////////////////////////////////
void Knock(){

  byte Digit = 0;

  byte KnockInDigit = 0;

  while (Digit < sizeof(Combination)){

    while (KnockInDigit < Combination[Digit]){

      delay(KnockInDigitDelay);

      digitalWrite(Solenoid, HIGH);

      delay(SolenoidOnDelay);

      digitalWrite(Solenoid, LOW);

      KnockInDigit++;

    }

    KnockInDigit = 0;

    delay(KnockBetweenDigitDelay);

    Digit++;

  }

  Mode = 0;

}

///////////////////////////////////////////////////////////
void loop() {

  switch (Mode){

    case 0:

      //Delay

      delay(500);

      // Look for  cards
      if (rfid.PICC_IsNewCardPresent()) {

        Serial.println("Card found.");

        Mode = 1;

      } else if (!rfid.PICC_IsNewCardPresent()) {

          Serial.println("No card.");

          DoKnocking = true;

      }

      break;

    case 1: //Do the knock

      Serial.println("Called knock.");

      if (DoKnocking) Knock();

      DoKnocking = false;

      break;

  }
}
