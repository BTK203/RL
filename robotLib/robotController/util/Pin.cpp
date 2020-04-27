#include "../RobotController.h"

/**
 * Source file for the Pin class.
 * Written By: Brach Knutson
 */

Pin::Pin(int pinNumber) {
	this->pinNumber = pinNumber;
	this->blinkRate = 500;
	this->state = PinState::OFF;
	
	pinMode(this->pinNumber, OUTPUT);
}

void Pin::Update() {
	switch(this->state) {
		case PinState::OFF:
			digitalWrite(this->pinNumber, LOW);
			break;
		case PinState::STEADY:
			digitalWrite(this->pinNumber, HIGH); 
			break;
		case PinState::BLINK: {
			long time = RobotController::GetClockTime();
			double trueBlinkRate = (double) (blinkRate * 2);
			double blinkValue = (time % (int) trueBlinkRate) / trueBlinkRate;
			if(blinkValue < 0.5) {
				digitalWrite(this->pinNumber, HIGH);
			} else {
				digitalWrite(this->pinNumber, LOW);
			}
			break;
		}
	}
}

void Pin::SetState(PinState state) {
	this->state = state;
}

PinState Pin::GetState() {
	return this->state;
}

void Pin::SetBlinkRate(int rate) {
	this->blinkRate = rate;
}

int Pin::GetBlinkRate() {
	return this->blinkRate;
}
