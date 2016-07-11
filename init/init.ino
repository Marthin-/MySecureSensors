#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <Wire.h>

#define GATEWAY //comment/uncomment to select between gateway and node mode
#define READ_CONFIG //in case you want to know what you secured network is made of ;)
#define DEBUG //more verbose
//#define TRANSMISSION //for debug purpose. Does not stop transmission in node mode.
#define GENERATE_AES //save space in case of low security need
#ifdef GENERATE_AES
  #include <AES.h>
#endif


//for data reading
#define NODE_ID_ADDR 0
#define IV_ADDR 1
#define KEY_ADDR 9
#define DESC_ADDR 41
#define DATA_SIZE 141

AES aes;
unsigned long long int IV;
byte buff[40];
byte nodeID; //A modifier plus tard
File myFile;
byte AES_256_key[32];
byte buff_8[8];
byte descr[100];


void setup() {
	Serial.begin(9600);
 #ifdef DEBUG
	Serial.println("============= MySensors =============");
	Serial.println("======= Secured Device Pairing ======");
#endif

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

  nodeID=101;
#ifdef TRANSMISSION
#ifdef DEBUG
  Serial.println("Asking node for nodeID...");
#endif
	Wire.requestFrom(8,1);
	while(Wire.available()){
		nodeID=Wire.read();
	}

#ifdef DEBUG
  Serial.println("Asking node for description...");
#endif
  byte cpt=0;
  Wire.requestFrom(8,100);
  while(Wire.available()){
    descr[cpt]=Wire.read();
    cpt++;
  }
#endif
  Serial.print("Adding node number ");
	Serial.print(nodeID);
	Serial.println(" to secured MySensors");



  /*************** Check duplication ********************/
  
#ifdef DEBUG
  Serial.println("Checking file for duplication...");
  Serial.println("Initializing SD card...");
#endif  
  if (!SD.begin()) {
#ifdef DEBUG
    Serial.println("Initialization failed!");
#endif
    return;
  }
  myFile=SD.open("SENSBDD.TXT");
  unsigned int count=NODE_ID_ADDR;
  myFile.seek(0);
  for(unsigned int count=0;count<myFile.size();count+=DATA_SIZE){
#ifdef DEBUG
    Serial.println("reading byte n°");
    Serial.print(count);
    Serial.println("");
#endif
    if(!myFile.available()){
      break;
    }
    myFile.seek(count);
    if((char)myFile.read()==(char)nodeID){
      Serial.println("Error adding sensor : NodeID already present in file. Please remove previous sensor or change nodeID");
      return;
    }
  }
  myFile.close();
  Serial.println("No duplication found.");



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
#ifdef TRANSMISSION
	Serial.println("Sending IV and AES key through I2C");
  Wire.beginTransmission(8);
	Serial.print("Sent buffer : ");
	memcpy(buff,(byte*)&IV,8);
	memcpy(buff+8,AES_256_key,32);
	Serial.write(buff,40);
	Wire.write(buff,40);
	Wire.endTransmission();
	Serial.println("\n[OK]\n");
#endif

	/** Storing generated key and device information on SD card **/

  //encoded data : nodeID/IV/key/desc
  //data size : 141 bytes
  byte desc[100]="Tartiflette officieuse";
  byte myData[DATA_SIZE];
  memcpy(myData+NODE_ID_ADDR,(byte*)&nodeID,1); //little trick to write nodeID correctly
  memcpy(myData+IV_ADDR,(byte*)IV,8);
  memcpy(myData+KEY_ADDR,AES_256_key,32);
  memcpy(myData+DESC_ADDR,desc,100);
  Serial.print("Data written into SD card : ");
  Serial.write(myData,141);
  Serial.println("");
  
	Serial.println("Storing data on SD card...");
	Serial.println("Initialization done.");
  myFile = SD.open("SENSBDD.TXT",FILE_WRITE);
  if (myFile) {
    myFile.seek(myFile.size());
    myFile.write(myData,141);
  } 
  else {
    Serial.println("error opening MySecureSensors file");
  }
  myFile.flush();
  myFile.close();
  Serial.println("[OK]");



 
  #ifdef READ_CONFIG
    myFile=SD.open("SENSBDD.TXT");
    Serial.println("reading SENSBDD.TXT :");
    byte temp[32];
    byte temp2[8];
    byte temp3[100];
    Serial.println("Node ID|    IV    |               AES key             |           info");
    while(myFile.available()){
      Serial.print("    ");
      Serial.print(myFile.read());
      Serial.print("  | ");
      for(int i=0;i<8;i++){
        temp2[i]=myFile.read();
      }
      Serial.write(temp2,8);
      Serial.print(" | ");
      for(int i=0;i<32;i++){
        temp[i]=myFile.read();
      }
      Serial.write(temp,32);
      Serial.print(" | ");
      for(int i=0;i<100;i++){
        temp3[i]=myFile.read();
      }
      Serial.println((char*)temp3);
    }
    myFile.close();
  #endif

 
	/********************* END OF GATEWAY MODE *****************/
	/***************************NODE MODE **********************/
	/*** Send NodeID + receive & store AES key + IV into EEPROM*/

#else

	/******** Send node ID to gateway through I2C **************/
	Wire.begin(8);
	Serial.println("You may enter node ID manually (between 000 and 255, e.g. 001,212,054...");
	Serial.setTimeout(20000);
  byte nodeID_byte[3];
	if((Serial.readBytesUntil('\n',nodeID_byte,3))<1){  
		EEPROM.get(2,nodeID); 
	}
  else{
    nodeID=((nodeID_byte[0]-48)*100+(nodeID_byte[1]-48)*10+nodeID_byte[2]-48);
  }
#ifdef DEBUG
  Serial.print("Sending (parent) node ID to gateway : ");
  Serial.println(nodeID);	
#endif
  Wire.onRequest(sendNodeId);
#ifdef DEBUG
  Serial.println("[OK]");
#endif
  /****** send node description to gateway *******************/
  Serial.setTimeout(30000);
  Serial.println("Enter short device description");
  Serial.readBytesUntil('\n',descr,100);
  Serial.print("Sending description to gateway...\n");
  Wire.onRequest(sendDescription);
  Serial.println("[OK]");
  

	/******** Receive AES IV & key, store it into EEPROM *******/

#ifdef GENERATE_AES
	Wire.onReceive(receiveAes);
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
	Serial.println("\nReceived and stored (not human-readable) AES initialization vector :\n\"");
	Serial.write(buff_8,8);
	Serial.println("\n[OK]\n");
#endif  

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

void sendDescription(){
  Wire.write(descr,100);
}

#ifdef GENERATE_AES
void receiveAes(int howMany){
	//TODO
	byte i=0;
	/* store received byte array in buff (reduce, recycle, reuse ;) */
	while(Wire.available()){
		buff[i]=Wire.read();
		i++;
	}
}
#endif
