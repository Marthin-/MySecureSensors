#include <EEPROM.h>
#define MASTER_KEY_ADDR 0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("generating AES-128 random key");
  randomSeed(analogRead(0));
  Serial.println("Now generating...");
  byte key[32];
  for(int i=0;i<16;i++){
    key[i]=random(0,256);
  }
  Serial.print("generated key : ");
  for (int i=0;i<32;i++){
    Serial.print(key[i]);
  }
  Serial.println("");
  Serial.println("Storing key into EEPROM");
  int eepromAddress=MASTER_KEY_ADDR;
  EEPROM.put(eepromAddress,key);
}

void loop() {
  // put your main code here, to run repeatedly:
}
