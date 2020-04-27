#include "Util.h"

/**
 * Source file for the Clock class.
 * Written By: Brach Knutson
 */

using namespace std::chrono;


Clock::Clock() {
}


void Clock::Start() {
	this->lastTime = timeSinceEpoch();
	this->counter = 0;
}

void Clock::Iterate() {
	long currentTime = timeSinceEpoch();
	long timeSinceLastTime = abs(currentTime - lastTime);
	counter += timeSinceLastTime;
	lastTime = currentTime;
}

long Clock::getTimeSinceStart() {
	return counter;
}

long Clock::timeSinceEpoch() {
	milliseconds ms = duration_cast<milliseconds> (
		system_clock::now().time_since_epoch()
	);
	
	return ms.count();
}
