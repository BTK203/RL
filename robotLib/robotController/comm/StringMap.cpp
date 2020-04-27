#include "../RobotController.h"

/**
 * Source file for the StringMap class.
 * Written By: Brach Knutson
 */

StringMap::StringMap() {
	this->pairs = std::vector<StringPair>();
	this->lastUpdateTime = 0;
}


void StringMap::Set(std::string name, std::string value) {
	int index = indexOf(name);
	
	if(index < 0) { //must create a definition
		StringPair newPair = StringPair(name, value);
		this->pairs.push_back(newPair);
	} else {
		this->pairs[index].SetValue(value);
	}
	
	this->lastUpdateTime = RobotController::GetClockTime();
}


std::string StringMap::Get(std::string name) {
	int index = indexOf(name);
	
	if(index >= 0) {
		return this->pairs[index].GetValue();
	}
	
	return "";
}


std::vector<std::string> StringMap::GetNames() {
	std::vector<std::string> names;
	
	for(int i=0; i<this->pairs.size(); i++) {
		names.push_back(this->pairs[i].GetName());
	}
	
	return names;
}


long StringMap::GetLastUpdatedTime() {
	return this->lastUpdateTime;
}


int StringMap::indexOf(std::string str) {
	for(int i=0; i<this->pairs.size(); i++) {
		if(this->pairs[i].GetName() == str) {
			return i;
		}
	}
	
	return -1;
}
