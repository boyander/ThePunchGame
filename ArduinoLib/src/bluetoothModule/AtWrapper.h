/*
 * AtWrapper.h
 *
 *  Created on: 13/05/2012
 *      Author: boyander
 */


#include <arduino.h>

#ifndef ATWRAPPER_H_
#define ATWRAPPER_H_

class AtWrapper {
private:
	String name;
	void sendATCommand(String at);
public:
	AtWrapper();
	AtWrapper(String name);
	void init();
	void streamSerial();
	virtual ~AtWrapper();
};

#endif /* ATWRAPPER_H_ */
