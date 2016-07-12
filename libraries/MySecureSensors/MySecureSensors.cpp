void initiateSecureSensors(){
    #ifdef DEBUG
		Serial.println("============= MySensors =============");
		Serial.println("======= Secured Device Pairing ======");
	#endif
    #ifdef GATEWAY
		initiateGateway();
	#else
		initiateNode();
	#endif
}

void initiateGateway(){
	Serial.println("titi");
}

void initiateNode(){
	Serial.println("toto");
}
