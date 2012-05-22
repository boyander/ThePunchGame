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
	EBU_INPUTCONNREQUEST,
	EBU_UNDEFINED,
	EBU_NORESPONSE
};

#define NAME_MAX_LENGTH 18
#define RESPONSE_BUFFER 128

class AtWrapper {
private:
	String name;
	bool sendATCommand(String at);
public:

	String client;

	AtWrapper();
	AtWrapper(String name);
	bool connect(bool stream);
	int readInput(char* str,int buffer);
	rCode CatchResponse(bool debug);
	void waitForLink();
	virtual ~AtWrapper();
};
