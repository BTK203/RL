#include "Util.h"

/**
 * Source file for the Util class.
 * Written By: Brach Knutson
 */

int Util::indexOf(std::string str, char target, int start) {
	for(int i=start; i<str.length(); i++) {
		if(str.at(i) == target) {
			return i;
		}
	}
	
	return -1;
}


int Util::lastIndexOf(std::string str, char target, int start) {
	for(int i=start; i>=0; i--) {
		if(str.at(i) == target) {
			return i;
		}
	}
	
	return -1;
}


std::string Util::substring(std::string str, int begin, int end) {
	std::string newString = "";
	for(int i=begin; i<end; i++) {
		newString += str.at(i);
	}
	
	return newString;
}


bool Util::stringStartsWith(std::string str, std::string phrase) {
	for(int i=0; i<phrase.length(); i++) {
		if(str.at(i) != phrase.at(i)) {
			return false;
		}
	}
	
	return true;
}

/**
 * Splits A string into segments based on a passed divider.
 * @param str the string to split
 * @param divider the divider to split the string by.
 * @param results a pointer to an array to put the results into
 * @return how many segments the string was split into
 */
int Util::splitString(std::string str, char divider, std::string *result) {
	std::string currentBuffer = "";
	int instancesOfDivider = Util::instancesInString(str, divider);
	int currentInstance = 0;
	
	for(int i=0; i<str.length(); i++) {
		char currentChar = str.at(i);
		
		if(currentChar == divider) {
			result[currentInstance] = currentBuffer;
			currentBuffer = "";
			currentInstance++;
		} else {
			currentBuffer += currentChar;
		}
	}
	
	result[currentInstance] = currentBuffer;
	return instancesOfDivider + 1;
}


int Util::instancesInString(std::string searchString, char occurance) {
	int counter = 0;
	for(int i=0; i<searchString.length(); i++) {
		if(searchString.at(i) == occurance) {
			counter++;
		}
	}
	
	return counter;
}


double Util::roundTo(double value, int zeros) {
	double roundValue = pow(10, zeros);
	value *= roundValue;
	value = (int) value;
	value /= roundValue;
	return value;
}


void Util::err(const char *msg) {
	perror(msg);
}
