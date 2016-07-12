#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <Wire.h>

#define GATEWAY //comment/uncomment to select between gateway and node mode
#define READ_CONFIG //in case you want to know what you secured network is made of ;)
//#define DEBUG //more verbose
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

void initiateSecureSensors();
void initiateGateway();
void initiateNode();
