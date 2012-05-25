

#include <arduino.h>
#include "bluetoothModule/AtWrapper.h"

int led = 13;

AtWrapper wp("ThePunchGame");

void setup(){

	Serial.begin(115200);

	//Blocks until conection income
	if(wp.connect(true)){
		//Blink led showing conection ok
		pinMode(led, OUTPUT);
		digitalWrite(led,HIGH);
		delay(1000);
		digitalWrite(led,LOW);
	}
}

void loop(){
	Serial.println("Connected with -> " + wp.client);
	delay(2000);
}
