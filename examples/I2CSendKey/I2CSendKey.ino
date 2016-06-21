#include <Wire.h>
#include <EEPROM.h>

  String key="fonzie42";


void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Serial.begin(9600);
  Serial.print("key to send : ");
  Serial.println(key);
  Wire.onRequest(requestEvent);
}
  
void loop() {
  // put your main code here, to run repeatedly:

}
void requestEvent() {
  Wire.write("fonzie42"); // respond with message of 6 bytes
  // as expected by master
}

