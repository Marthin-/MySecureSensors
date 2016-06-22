#include "Rsa.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Generate RSA key, please wait...");
  Rsa rsa=Rsa();
  rsa.generateKey();
  Serial.println("Key generated!");
  Serial.print("Using (");
  Serial.print(rsa.getE());
  Serial.print(",");
  Serial.print(rsa.getN());
  Serial.println(") as public key and");
  Serial.print("(");
  Serial.print(rsa.getD());
  Serial.println(") as private key");
}

void loop() {
  // put your main code here, to run repeatedly:

}
