#include <EEPROM.h>

int eepromAddress=0;

void setup() {
  Serial.begin(9600);
  Serial.println("reading string from eeprom and storing it");
  char buff[8];
  EEPROM.get(eepromAddress,buff);
  Serial.print("String read : \"");
  Serial.print(buff);
  Serial.println("\"");
}

void loop() {
  // put your main code here, to run repeatedly:

}
