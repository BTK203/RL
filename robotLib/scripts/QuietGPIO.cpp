#include <iostream>
#include "wiringPi.h"

/**
 * Source file for the QuietGPIO executable.
 * This executable sets all 40 GPIO pins to LOW (aka OFF). 
 * This executable is run before the robot program starts, so that 
 * pin readouts are accurate.
 */
 
const int COUNT = 40;

int main(int argc, char *argv[]) {
	wiringPiSetup();
	
	for(int i=0; i<COUNT; i++) {
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}
	
	return 0;
}
