

#include <arduino.h>
#include "bluetoothModule/AtWrapper.h"
#include "accelerometer/ACC.h"


int led = 13;

AtWrapper wp("ThePunchGame");

//Setup [X,Y,Z] for accelerometer
ACC acc(0,1,2);

void setup(){

	Serial.begin(115200);
	acc.calibrate();
	//Blocks until new conection request
	/*if(wp.connect(true)){
		//Blink led showing conection ok
		pinMode(led, OUTPUT);
		digitalWrite(led,HIGH);
		delay(1000);
		digitalWrite(led,LOW);
	}*/
}

void loop(){
	//Serial.println("Connected with -> " + wp.client);
	acc.printACCForces();
	delay(100);
}
