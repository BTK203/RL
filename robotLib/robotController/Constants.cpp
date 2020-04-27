#include "RobotController.h"

/**
 * Source file for the Constants class.
 * Written By: Brach Knutson
 */

const int Constants::INPUT_PORT  = 9230;
const int Constants::OUTPUT_PORT = 9231;
const int Constants::VIDEO_PORT  = 9232;
const int Constants::SECURE_PORT = 9233;

const int Constants::THREAD_LOOP_DELAY = 1;

const char *Constants::DEFAULT_ADDRESS = "192.168.0.1";
const char *Constants::DEFAULT_DESTINATION = "192.168.0.255";

const char Constants::BEGIN_CHARACTER = '(';
const char Constants::END_CHARACTER = ')';
