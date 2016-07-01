#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#define GATEWAY
#include <EEPROM.h>
#include <AES.h>

AES aes;
unsigned long long int IV;
byte buff[40];
byte nodeID; //A modifier plus tard
File myFile;
byte AES_256_key[32];

void setup() {
  Serial.begin(9600);
  Serial.println("============= MySensors =============");
  Serial.println("======= Secured Device Pairing ======");
  
/****************************************************************/
//
//       GATEWAY MODE : generate & send through I2C           
//
/****************************************************************/
  
  
  #ifdef GATEWAY
  Serial.println("======= Gateway mode: engaged =======");
  //for I2C purpose
  Wire.begin();  
  
/******* Ask node for node ID so that it can be added ***********/  

  Wire.requestFrom(8,1);
  while(Wire.available()){
    nodeID=Wire.read();
  }
  Serial.print("Adding node number ");
  Serial.print(nodeId);
  Serial.println(" to secured MySensors");
/***** Generate IV & AES key + Storing IV into EEPROM ***********/
  
  Serial.println("Generating Initialization Vector");
  IV=aes.generate_IV();
  Serial.println("[OK]\n");
  
  Serial.print("Generating AES-256 key");
    aes.generate_main_key(AES_256_key);
    Serial.println("[OK]\n");
    Serial.print("Generated key (mostly unreadable as is) : \"");
    Serial.write(AES_256_key,32);
  
  Serial.println("\"\nStoring IV into EEPROM"); 
    EEPROM.put(419,IV);
    Serial.println("[OK]\n");

/****** Sending IV & AES key as one byte array through I2C *****/
  
  Serial.println("Sending IV and AES key through I2C");
    Wire.beginTransmission(8);
    Serial.print("Sent buffer : ");
    memcpy(buff,(byte*)IV,8);
    memcpy(buff+8,AES_256_key,32);
    Serial.write(buff,40);
    Wire.write(buff,40);
    Wire.endTransmission();
    Serial.println("\n[OK]\n");

/** Storing generated key and device information on SD card **/
  
  Serial.println("Storing AES key on SD card");
    Serial.print("Initializing SD card...");
    if (!SD.begin()) {
      Serial.println("initialization failed!");
      return;
    }
  Serial.println("initialization done.");
  myFile = SD.open("SENSBDD.TXT",FILE_WRITE);
  if (myFile) {
    Serial.println("MySecureSensors file (SENSBDD.TXT) : ");
    myFile.seek(myFile.size());
    myFile.println("");
    myFile.println(nodeID);
    myFile.write(AES_256_key,32);
    myFile.println("");
    myFile.println("test write AES key");
    myFile.close();
  } else {
    Serial.println("error opening MySecureSensors file");
  }
  Serial.println("Reading MySecureSensors file");
  myFile=SD.open("SENSBDD.TXT");
  if(myFile){
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
  }
  myFile.close();
  Serial.println("[OK]");

/********************* END OF GATEWAY MODE *****************/
/***************************NODE MODE **********************/
/*** Send NodeID + receive & store AES key + IV into EEPROM*/
  
  #else
  Serial.println("======== Node mode: engaged =========");

/******** Send node ID to gateway through I2C **************/
  Wire.begin(8);
  EEPROM.get(2,nodeID);
  Wire.onRequest(sendNodeId);
  Serial.print("Sending (parent) node ID to gateway : ");
  Serial.println(nodeID);
  Serial.println("[OK]\n");
  
/******** Receive AES IV & key, store it into EEPROM *******/

  Wire.onReceive(receiveAes);
  byte buff_8[8];
  for(int i=0;i<8;i++){
    buff_8[i]=buff[i];
  }
  for(int i=8;i<40;i++){
    AES_256_key[i-8]=buff[i];
  }
  EEPROM.put(396,AES_256_key);  //396 = EEPROM_RF_ENCRYPTION_AES_KEY_ADDRESS
  EEPROM.put(429,buff_8);       //429 = EEPROM_RF_ENCRYPTION_AES_IV_ADDRESS
  Serial.print("Received and stored (not human-readable) AES 256 bit key :\n\"");
  Serial.write(AES_256_key,32);
  Serial.println("\"\nReceived and stored (not human-readable) AES initialization vector :\n\"");
  Serial.write(buff_8,8);
  Serial.println("\"\n[OK]\n");
  
  
  #endif
  Serial.println("=====================================");
  Serial.println("You can now upload your MySensors \ncode and enjoy your secure network ;)");
}

void loop() {
  // empty loop
}

void sendNodeId(){
  Wire.write(nodeID);
}

void receiveAes(){
  //TODO
  byte i=0;
  /* store received byte array in buff (reduce, recycle, reuse ;) */
  while(Wire.available()){
    buff[i]=Wire.read();
    i++;
  }
}
