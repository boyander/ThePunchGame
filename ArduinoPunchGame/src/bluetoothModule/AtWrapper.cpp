/*
 * AtWrapper.cpp
 *
 *  Created on: 13/05/2012
 *      Author: boyander
 */

#include "AtWrapper.h"

AtWrapper::AtWrapper() {

}

AtWrapper::AtWrapper(String name) {
	if(name){
		this->name = name;
	}else{
		this->name = "";
	}
}

void AtWrapper::init(){
	Serial.begin(115200);


	// Reset hardware
	sendATCommand("AT+JRES");

	// Enable security command
	sendATCommand("AT+JSEC=1,1,1,04,1111");

	// Discorable command
	sendATCommand("AT+JDIS=3");

	if( name != "" && name.length() < 18){
		// Set friendly name for bluetooth
		sendATCommand("AT+JSLN="+String(name.length())+", "+name);
	}

	// Register local sevice command
	sendATCommand("AT+JRLS=1101,11,Serial Port,01,000000");

	// Auto accept connection requests command
	sendATCommand("AT+JAAC=1");

	// Flush serial buffer
	Serial.flush();
}

void AtWrapper::streamSerial(){
	sendATCommand("AT+JSCR");
}

void AtWrapper::sendATCommand(String at){
	delay(2000);
	Serial.print(at+"\r\n");
}


AtWrapper::~AtWrapper() {
	// TODO Auto-generated destructor stub
}

