/*
* THE PUNCH GAME
* UPC, EET - Emitters and Receivers
* author: MPomar,JDomenech,VHidalgo,AOlive,HGallego
* @repo: https://github.com/boyander/ThePunchGame
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

#define DEBUG_RESPONSE false

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


