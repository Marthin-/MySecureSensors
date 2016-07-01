Use this sketch to add sensor to gateways whitelist :
-gw side :
  -generates AES 256 bit key
  -generates AES IV (8 bit)
  -gets NodeID from sensor through I2C
  -stores IV into EEPROM
  -Add node to whitelist on SD card (file may be edited manually, though not encouraged)
  -sends AES key and IV through I2C to node
  
-node side :
  -sends NodeID to gw through I2C
  -receives AES key & IV from gw
  -stores it into EEPROM
