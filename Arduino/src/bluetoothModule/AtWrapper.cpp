/*
 * AtWrapper.cpp
 *
 *  Created on: 13/05/2012
 *      Author: boyander
 */

#include "AtWrapper.h"

AtWrapper::AtWrapper() {
	Serial.flush();
}

AtWrapper::AtWrapper(String name) {
	if(name){
		this->name = name;
	}else{
		this->name = "";
	}
	AtWrapper();
}

bool AtWrapper::connect(bool stream = true){

	// Reset hardware device
	if( !sendATCommand("AT+JRES") ) return false;

	// Enable security command
	if( !sendATCommand("AT+JSEC=1,1,1,04,1111") ) return false;

	// Discoverable command
	if( !sendATCommand("AT+JDIS=3") ) return false;


	// Name validation
	if( name == "" ){
		// Set friendly name for bluetooth
		this->name = "BluetoothSerial";
	}else if (name.length() > NAME_MAX_LENGTH){
		// Trim name
		this->name = this->name.substring(0,NAME_MAX_LENGTH);
	}

	//Configure device name
	if( !sendATCommand("AT+JSLN="+String(name.length())+","+name) ) return false;

	// Register local sevice command
	if( !sendATCommand("AT+JRLS=1101,11,Serial Port,01,000000") ) return false;

	// Auto accept connection requests command
	if( !sendATCommand("AT+JAAC=1") ) return false;

	//Wait for +RCOI response from device
	waitForLink();

	if (stream){
		//Stream serial command (after this command it's a plain serial connection)
		if( !sendATCommand("AT+JSCR") ) return false;
	}

	//All was ok, so start playing!
	return true;
}


int AtWrapper::readInput(char* str,int buffer){
	int inByte = -1, lastByte = -1;
	int string_len = 0;

	while( (inByte == -1 || Serial.available() > 0) && (string_len < buffer)){
	   delay(10); // let the buffer fill up
	   // Read new byte
	   inByte = (char) Serial.read();
	   //Detect LF + CR (\n\r)
	   if((char)inByte == '\n') return string_len;
	   //Read incoming byte
	   if(inByte != -1){
		   str[string_len] = (char)inByte; // Save the data in a character array
		   string_len++;
	   }
	   //Store last byte
	   lastByte = inByte;
	}
	return string_len;
}

rCode AtWrapper::CatchResponse(bool debug = false) {

	char in[RESPONSE_BUFFER] = {0};
	int bytesReaded = 0;
	bytesReaded = readInput((char*) in, RESPONSE_BUFFER);

	String input(in);

	if(bytesReaded > 0){

		//Debug received command from device
		if(debug){
			Serial.print("Input was: ");
			Serial.print(input);
			Serial.println("");
		}

		//Parse received command
		if(input.substring(0,3) == "ROK"){
			return EBU_RESETOK;
		} else if (input.substring(0,2) == "OK") {
			return EBU_SETTEDOK;
		} else if (input.substring(0,5) == "+RCOI") {
			this->client = input.substring(6,19);
			return EBU_INPUTCONNREQUEST;
		} else if (input.substring(0,3) == "ERR") {
			return EBU_ERROR;
		}

		return EBU_UNDEFINED;
	}
	return EBU_NORESPONSE;
}

void AtWrapper::waitForLink(){
	//Loop until response
	rCode r = EBU_NORESPONSE;
	while(r != EBU_INPUTCONNREQUEST){
		r = CatchResponse();
	}
}

bool AtWrapper::sendATCommand(String at){

	//Send AT Code
	Serial.print(at+"\r\n");

	//Loop until response
	rCode r = EBU_NORESPONSE;
	while(r == EBU_NORESPONSE){
		r = CatchResponse();
	}

	//Flush serial connection buffers
	Serial.flush();

	//Return true if positive response or reset
	if( r == EBU_SETTEDOK || r == EBU_RESETOK ){
		return true;
	}

	//Check response code and inform user if there was an error
	switch(r){
		case EBU_ERROR:
			Serial.println("Error on AT Command!");
		break;
		case EBU_UNDEFINED:
			Serial.println("Undefined AT Command!");
		break;
		default:
		break;
	}

	//Unexpected response
	return false;
}


AtWrapper::~AtWrapper() {
	// TODO Auto-generated destructor stub
}

