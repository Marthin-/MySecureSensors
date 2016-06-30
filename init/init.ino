#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#define GATEWAY
#include <EEPROM.h>
//#define MY_RADIO_NRF24
//#define MY_DEBUG
//#include <MySensors.h>
#include <AES.h>



AES aes;
unsigned long long int IV;
byte to_send[40];
byte nodeID=1; //A modifier plus tard
File myFile;
byte AES_256_key[32];

void setup() {
  // put your setup code here, to run once:


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
  Wire.begin(8);  

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
    Wire.onRequest(sendI2C);
    Serial.print("Sent buffer : ");
    memcpy(to_send,(byte*)IV,8);
    memcpy(to_send+8,AES_256_key,32);
    Serial.write(to_send,40);
    
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
  //TODO

  
  #endif
  Serial.println("=====================================");
  Serial.println("You can now upload your MySensors code and enjoy your secure network ;)");
}

void loop() {
  // put your main code here, to run repeatedly:

}

// send IV + AES key through I2C
void sendI2C(){
  Wire.write(to_send,40);
}

