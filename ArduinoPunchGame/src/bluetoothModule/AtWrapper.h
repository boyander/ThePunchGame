/*
 * AtWrapper.h
 *
 *  Created on: 13/05/2012
 *      Author: boyander
 */


#include <arduino.h>

enum rCode{
	EBU_RESETOK,
	EBU_SETTEDOK,
	EBU_ERROR,
	EBU_UNDEFINED,
	EBU_NORESPONSE
};

#define RESPONSE_BUFFER 128

class AtWrapper {
private:
	String name;
	void sendATCommand(String at);
public:
	AtWrapper();
	AtWrapper(String name);
	void init();
	void streamSerial();
	void readInput(char* str,int buffer);
	rCode CatchResponse();
	void waitForLink();
	virtual ~AtWrapper();
};
