#include <Wire.h>
#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  byte to_write[8];
  byte c;
  byte i=0;
  Wire.requestFrom(8,8);
  while(Wire.available()){
    if(i<8){
      c=Wire.read();
      to_write[i]=c;
      i++;
    }
  }
  Serial.print("received key : ");
  for(int j=0;j<8;j++){
    Serial.print((char)to_write[j]);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
