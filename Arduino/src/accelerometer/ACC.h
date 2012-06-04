/*
* THE PUNCH GAME
* UPC, EET - Emitters and Receivers
* author: MPomar,JDomenech,VHidalgo,AOlive,HGallego
* @repo: https://github.com/boyander/ThePunchGame
*/

#include <arduino.h>
#include <math.h>

#define ACC_X 0
#define ACC_Y 1
#define ACC_Z 2

#define ADC_RESOLUTION_BITS 10

/* ADXL335 default sensitivity in mV/g */
#define ADXL335 300

class ACC {
	private:
		/* Analogic read pins for input data, by default [X,Y,Z] */
		int* pins;

		/* Analogic ADC readed data */
		int *data;

		/* Zero Gravity reference point in mV (calibrated values) */
		float *zerog;

		/* Sensitivity in mV/g */
		float sensitivity;

		/* ADC REF Voltage in mV */
		int reference;

		/*Reads RAW data from ADC on selected Pins*/
		void readRAWData();
	public:
		ACC();
		ACC(int pinX, int pinY, int pinZ);
		void calibrate();
		void printRAWData();
		void printACCForces();
};

