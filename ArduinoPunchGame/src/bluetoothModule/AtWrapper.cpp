/*
 * AtWrapper.cpp
 *
 *  Created on: 13/05/2012
 *      Author: boyander
 */

#include "AtWrapper.h"

AtWrapper::AtWrapper() {
	Serial.flush();
	delay(250);
}

AtWrapper::AtWrapper(String name) {
	if(name){
		this->name = name;
	}else{
		this->name = "";
	}
	AtWrapper();
}

void AtWrapper::init(){

	// Reset hardware
	sendATCommand("AT+JRES");

	// Enable security command
	sendATCommand("AT+JSEC=1,1,1,04,1111");

	// Discorable command
	sendATCommand("AT+JDIS=3");


	if( name != "" && name.length() < 18){
		// Set friendly name for bluetooth
		sendATCommand("AT+JSLN="+String(name.length())+","+name);
	}

	// Register local sevice command
	sendATCommand("AT+JRLS=1101,11,Serial Port,01,000000");

	//sendATCommand("AT+JCCR=00191D74A8EC,01");

	// Auto accept connection requests command
	sendATCommand("AT+JAAC=1");
	waitForLink();
}


int AtWrapper::readInput(char* str,int buffer){
	char inByte = -1, lastByte = -1;
	int string_len = 0;

	while( (inByte == -1 || Serial.available() > 0) && (string_len < buffer)){
	   //Detect LF + CR (\n\r)
	   //if( lastByte == 10 && inByte == 13) return;
	   //Read incoming byte
	   if(inByte != -1){
		   str[string_len] = (char)inByte; // Save the data in a character array
		   string_len++;
	   }
	   //Store last byte
	   lastByte = inByte;
	   // Read new byte
	   inByte = (char) Serial.read();

	}
	return string_len;
}

rCode AtWrapper::CatchResponse() {

	char in[RESPONSE_BUFFER] = {0};
	int bytesReaded = 0;
	bytesReaded = readInput((char*) in, RESPONSE_BUFFER);

	String input(in);

	if(bytesReaded > 0){
		//Print input
		Serial.print(in);

		/*//Check for received code
		if (strcmp("ROK", in) < 0) {
			return EBU_RESETOK;
		} else if (strcmp("OK", in) < 0) {
			return EBU_SETTEDOK;
		} else if (strcmp("ERR=", in) < 0) {
			return EBU_ERROR;
		}*/

		return EBU_UNDEFINED;
	}
	return EBU_NORESPONSE;
}

void AtWrapper::waitForLink(){
	char in[RESPONSE_BUFFER];
	readInput((char*) in, RESPONSE_BUFFER);
	while(true){
		if(strcmp("+RCOI", in) < 0){
			return;
		}else{
			readInput((char*) in, RESPONSE_BUFFER);
		}
	}
}

void AtWrapper::streamSerial(){
	sendATCommand("AT+JSCR");
}

void AtWrapper::sendATCommand(String at){

	//Send AT Code
	Serial.print(at+"\r\n");

	//Loop until response
	rCode r;
	while(r == EBU_NORESPONSE){
		r = CatchResponse();
	}

	//Check response code and inform user if there was an error
	if ( r == EBU_ERROR || r == EBU_UNDEFINED){
		Serial.println("Error on AT Command!");
	}

	//Flush serial connection buffers
	Serial.flush();
}


AtWrapper::~AtWrapper() {
	// TODO Auto-generated destructor stub
}

