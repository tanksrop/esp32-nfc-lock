#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>
#include "users.h"
Users userlist;
#include "Serial_Commands.h"
serialCommands commands;

const char* error_msgs[5] = {"Success!", "User doesn't exist!", "Invalid user permissions!", "Userlist Full", "no users remaining"};
uint8_t error;
int activationTime = 100; //ms to run relay for
int activationSleep = 500; // time to pause after successful unlock
int required_permission_level = 4; //minimum level to unlock (0-4 0 being master and 4 being lowest)

#define SS_PIN 5                  // The SS pin is connected to Pin 10
#define RST_PIN 15                  // the RST pin is conected tp pin 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
int relay1 = 22;                    // relay 1

void authorised() {
    digitalWrite(relay1, HIGH);
    delay(activationTime);
    digitalWrite(relay1, LOW);
    delay(activationSleep);
  }

void setup() {
  pinMode(relay1, OUTPUT);    // Declaring this Relay as output
  digitalWrite(relay1, LOW);  // setting it to OFF
  Serial.begin(115200);  // Initiate a serial communication
  SPI.begin();           // Initiate  SPI bus
  mfrc522.PCD_Init();    // Initiate MFRC522
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max); // Set to max gain for range
  Serial.println(); // empty line

  Serial.println(F("id\tuid\t\t\tpermission_level\tname"));
  for(int i = 0; i < 20; i++)
  {
    Serial.print(i +1);
    Serial.print(F("\t"));
    Serial.print(userlist.data[i].uid);
    Serial.print(F("\t\t\t"));
    Serial.print(userlist.data[i].perm);
    Serial.print(F("\t"));
    Serial.println(userlist.data[i].name);
  }
  preferences.begin("config", false);
  error = userlist.load();
  Serial.println(error_msgs[error]);
}

  void loop() {
    if (Serial.available()) {
      commands.commands();
    }
    // Awaiting an UID to be Presented
    if (!mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    //Display UID on serial monitor
    Serial.print("UID tag :");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();

    //check access
    String uid = content.substring(1);
    error = userlist.hasAccess(uid, required_permission_level);
        if (error == 0){
            authorised();
            Serial.println(error_msgs[error]);
        }
        else{
            Serial.println(error_msgs[error]);
        }
  }