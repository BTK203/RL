#include "Comm.h"

/**
 * Source file for the ButtonPair class.
 * Written By: Brach Knutson
 */

ButtonPair::ButtonPair(Button button, double value) {
	this->button = button;
	this->value = value;
}


Button ButtonPair::GetButton() {
	return this->button;
}


double ButtonPair::GetValue() {
	return this->value;
}


void ButtonPair::SetValue(double value) {
	this->value = value;
}
