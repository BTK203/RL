#include "Comm.h"

/**
 * Source file for the StringPair class.
 * Written By: Brach Knutson
 */
		
StringPair::StringPair(std::string name, std::string value) {
	this->name = name;
	this->value = value;
}


std::string StringPair::GetName() {
	return this->name;
}


std::string StringPair::GetValue() {
	return this->value;
}


void StringPair::SetValue(std::string value) {
	this->value = value;
}
