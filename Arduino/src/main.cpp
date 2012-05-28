

#include <arduino.h>
#include "bluetoothModule/AtWrapper.h"
#include "accelerometer/ACC.h"


int led = 13;

AtWrapper wp("ThePunchGame");

//Setup [X,Y,Z] for accelerometer
ACC acc(0,1,2);

void setup(){

	Serial.begin(115200);
<<<<<<< HEAD
	acc.calibrate();
	//Blocks until new conection request
	/*if(wp.connect(true)){
=======

	//Blocks until new conection request
	if(wp.connect(true)){
>>>>>>> 6625c833fe699a877946e3acdd61a5268d8e78d3
		//Blink led showing conection ok
		pinMode(led, OUTPUT);
		digitalWrite(led,HIGH);
		delay(1000);
		digitalWrite(led,LOW);
	}*/
}

void loop(){
<<<<<<< HEAD
	//Serial.println("Connected with -> " + wp.client);
	acc.printACCForces();
=======
	Serial.println("Connected with -> " + wp.client);
	acc.readData();
	acc.printData();
>>>>>>> 6625c833fe699a877946e3acdd61a5268d8e78d3
	delay(100);
}
