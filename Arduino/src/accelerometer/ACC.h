/*
 * ACC.h
 *
 *  Created on: 23/05/2012
 *      Author: boyander
 */

#include <arduino.h>

#define ACC_X 0
#define ACC_Y 1
#define ACC_Z 2

class ACC {
	private:
		/* Analogic read pins for input data, by default [X,Y,Z] */
		int* pins;
		/* Analogic readed data */
		int *data;
	public:
		ACC();
		ACC(int pinX, int pinY, int pinZ);
		void readData();
		void printData();
};

