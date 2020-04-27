#include "../robotController/RobotController.h"

/**
 * Main Source file for a RobotLib project.
 * Written By: Brach Knutson
 * 
 * This file should NOT be modified unless it is to change the 
 * indicator pin IDs. All it serves to do is instantiate the Robot 
 * Controller, which handles all function. All functionality
 * can and should be written into the Robot class.
 */

static const int
	PROGRAM_PIN = 8,
	RADIO_PIN   = 9,
	STATUS_PIN  = 7;

int main(int argc, char *argv[]) {
	RobotController::Init(PROGRAM_PIN, RADIO_PIN, STATUS_PIN);
}
