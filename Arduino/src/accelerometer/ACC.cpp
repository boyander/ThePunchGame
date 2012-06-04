/*
* THE PUNCH GAME
* UPC, EET - Emitters and Receivers
* author: MPomar,JDomenech,VHidalgo,AOlive,HGallego
* @repo: https://github.com/boyander/ThePunchGame
*/

#include "ACC.h"

ACC::ACC() {
	//analogReference(EXTERNAL);
	sensitivity = 300;
	reference = 5000;
}

ACC::ACC(int pinX, int pinY, int pinZ) {

	//Initialize pins to read
	this->pins = (int *) malloc(sizeof(int)*3);
	this->pins[ACC_X] = pinX;
	this->pins[ACC_Y] = pinY;
	this->pins[ACC_Z] = pinZ;

	//Initialize data vector
	this->data = (int *) malloc(sizeof(int)*3);

	//Initialize calibrated values
	this->zerog = (float *) malloc(sizeof(float)*3);

	analogReference(DEFAULT);
	sensitivity = 300;
	reference = 5000;
}

void ACC::readRAWData(){
	//Read data from Arduino ADC
	this->data[ACC_X] = analogRead(this->pins[ACC_X]);
	this->data[ACC_Y] = analogRead(this->pins[ACC_Y]);
	this->data[ACC_Z] = analogRead(this->pins[ACC_Z]);
}

void ACC::printRAWData(){
	Serial.print("ACC_X-> ");
	Serial.print(this->data[ACC_X],DEC);
	Serial.print("\tACC_Y-> ");
	Serial.print(this->data[ACC_Y],DEC);
	Serial.print("\tACC_Z-> ");
	Serial.print(this->data[ACC_Z],DEC);
	Serial.println("");
}


void ACC::calibrate(){
	this->zerog[ACC_X] = ((float)analogRead(this->pins[ACC_X]) * reference / 1023.0);
	this->zerog[ACC_Y] = ((float)analogRead(this->pins[ACC_Y]) * reference / 1023.0);
	this->zerog[ACC_Z] = ((float)analogRead(this->pins[ACC_Z]) * reference / 1023.0);
}


void ACC::printACCForces(){

	readRAWData();

	//printRAWData();

	/*Serial.print("ZERO X-> ");
	Serial.print(this->zerog[ACC_X]);
	Serial.print("\tZERO Y-> ");
	Serial.print(this->zerog[ACC_Y]);
	Serial.print("\tZERO Z-> ");
	Serial.print(this->zerog[ACC_Z]);
	Serial.println("");*/

	float RX = (((float)this->data[ACC_X] * reference / 1023.0) - this->zerog[ACC_X] / sensitivity);
	float RY = (((float)this->data[ACC_Y] * reference / 1023.0) - this->zerog[ACC_Y] / sensitivity);
	float RZ = (((float)this->data[ACC_Z] * reference / 1023.0) - this->zerog[ACC_X] / sensitivity);


	//float R = sqrt(RX*RX+RY*RY+RZ*RZ);

	//Angles
	//float CX = this->data[ACC_X] - this->zerog[ACC_X];
	//float CY = this->data[ACC_Y] - this->zerog[ACC_Y];
	//float CZ = this->data[ACC_Z] - this->zerog[ACC_Z];

	float pitch = atan2(RX,sqrt(RY*RY+RZ*RZ)) * 180. / M_PI;
	//float roll = atan2(RZ,RX)/ M_PI * 180.0;
	//float AX = acos(RX/R) * 180. / M_PI;
	//float AY = acos(RY/R) * 180. / M_PI;
	//float AZ = acos(RZ/R) * 180. / M_PI;

	/*Serial.print("G-> ");
	Serial.print(R);
	Serial.print("RX-> ");
	Serial.print(RX);
	Serial.print("\tRY-> ");
	Serial.print(RY);
	Serial.print("\tRZ-> ");
	Serial.print(RZ);
	Serial.println("");*/

	Serial.print(pitch);
	Serial.print(";");
	Serial.print(RY);
	Serial.print(";");
	Serial.print(RZ);
	Serial.print(";");
	Serial.println("");

}




