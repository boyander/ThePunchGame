

#include <arduino.h>
#include "bluetoothModule/AtWrapper.h"

int led = 13;
int val = -1;

void setup(){
	AtWrapper wp("ThePunchGame");
	wp.init();
	pinMode(led, OUTPUT);
	digitalWrite(led,HIGH);
	Serial.flush();
	val = Serial.read();
	while (val != 'R'){
	  val = Serial.read();
	}
	wp.streamSerial();
}

void loop(){
	Serial.println("Hola desde Eclipse...");
	delay(2000);
}
