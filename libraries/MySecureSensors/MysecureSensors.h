#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <Wire.h>

#ifndef MysecureSensors_h
#define MysecureSensors_h
#define GATEWAY //comment/uncomment to select between gateway and node mode
#define READ_CONFIG //in case you want to know what you secured network is made of ;)
#define DEBUG //more verbose
//#define TRANSMISSION //for debug purpose
#ifdef GATEWAY
  #include <AES.h>
#endif

//for data reading
#define NODE_ID_ADDR 0
#define IV_ADDR 1
#define KEY_ADDR 9
#define DESC_ADDR 41
#define DATA_SIZE 141

void initiateSecureSensors();
void initiateGateway();
void initiateNode();
byte requestNodeID();
void requestDescription(byte* buff);
boolean nodeIdAvailable(byte ID);
void sendCryptData();
void buildData(byte* myData);
void writeSD(byte* data);
void readSecureConfig();
void sendNodeID();
void sendDescription();
void receiveAes();

#endif
