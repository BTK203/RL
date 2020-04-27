#include "Comm.h"

/**
 * Source file for the ButtonMap class.
 * Written By: Brach Knutson
 */

ButtonMap::ButtonMap() {
	this->pairs = std::vector<ButtonPair>();
}


void ButtonMap::Set(Button button, double value) {
	int buttonIndex = indexOf(button);
	if(buttonIndex < 0) { //button has not been added to the map
		ButtonPair newPair = ButtonPair(button, value);
		this->pairs.push_back(newPair);
	} else {
		this->pairs[buttonIndex].SetValue(value);
	}
}


double ButtonMap::Get(Button button) {
	int buttonIndex = indexOf(button);
	if(buttonIndex >= 0) {
		return this->pairs[buttonIndex].GetValue();
	}
	
	return 0;
}


int ButtonMap::indexOf(Button button) {
	for(int i=0; i<this->pairs.size(); i++) {
		if(this->pairs[i].GetButton() == button) {
			return i;
		}
	}
	
	return -1;
}
