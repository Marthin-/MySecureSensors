# MySecureSensors
Add whole security protocol to MySensors Arduino library

TODO :
implemented | added to library

CONFIDENTIALITY
    ✔       |       ✔           generate random AES keys (128/256 bits) (previously RSA keys)
    ✔       |       ✖           exchange 256 bit key through I2C
    ✔       |       ✖           exchange AES IV through I2C
    ✔       |       ✖           store 256 bit key into eeprom (slave)
    ✔       |       ✖           store 256 bit key for each slave into SD card (gateway)
    ✖       |       ✖           generate 128 bit keys on the go + send it to slaves (gateway) using AES-256
    ✖       |       ✖           use sent keys to transmit messages

INTEGRITY
    ✖       |       ✖           generate hash from slave code (?)
    ✖       |       ✖           verify hash + blacklist if hash does not match
    
AUTHENTICITY : using Anticimex's implementation for signing messages    
