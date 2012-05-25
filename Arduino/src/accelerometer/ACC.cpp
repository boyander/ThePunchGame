/*
 * ACC.cpp
 *
 *  Created on: 23/05/2012
 *      Author: boyander
 */

#include "ACC.h"

ACC::ACC() {
	analogReference(DEFAULT);
}

ACC::ACC(int pinX, int pinY, int pinZ) {

	//Initialize pins to read
	this->pins = (int *) malloc(sizeof(int)*3);
	this->pins[ACC_X] = pinX;
	this->pins[ACC_Y] = pinY;
	this->pins[ACC_Z] = pinZ;

	//Initialize data vector
	this->data = (int *) malloc(sizeof(int)*3);

	//Setup with default constructor
	ACC();
}

void ACC::readData(){
	 //Read data
	this->data[ACC_X] = analogRead(this->pins[ACC_X]);
	this->data[ACC_Y] = analogRead(this->pins[ACC_Y]);
	this->data[ACC_Z] = analogRead(this->pins[ACC_Z]);
}

void ACC::printData(){
	Serial.print("ACC_X-> ");
	Serial.print(this->data[ACC_X],DEC);
	Serial.print("\tACC_Y-> ");
	Serial.print(this->data[ACC_Y],DEC);
	Serial.print("\tACC_Z-> ");
	Serial.print(this->data[ACC_Z],DEC);
	Serial.println("");
}
