

#include <arduino.h>
#include "bluetoothModule/AtWrapper.h"
#include "L2CAP/L2CAP.h"

int led = 13;

byte addr[6] = {0x00,0x19,0x1D,0x74,0xA8,0xEC};

void setup(){

	Serial.begin(115200);
	AtWrapper wp("ThePunchGame");

	wp.init();

	//Enable led
	pinMode(led, OUTPUT);
	digitalWrite(led,HIGH);
	delay(1000);
	digitalWrite(led,LOW);
	wp.streamSerial();
}

void loop(){
	Serial.println("Preueba");
	delay(2000);
	//l2cap.L2CAP_task();
}
