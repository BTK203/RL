#include "../RobotController.h"

/**
 * Source file for the OutputSocketManger class.
 * Written By: Brach Knutson
 */
 

OutputSocketManager::OutputSocketManager() {
	this->socket = DatagramSocket(Constants::DEFAULT_DESTINATION, Constants::OUTPUT_PORT);
}


OutputSocketManager::~OutputSocketManager() {
	this->socket.Close();
}


void OutputSocketManager::SendMessage(std::string message) {
	this->socket.Send(message);
}

/**
 * Sends the passed StringMap to the computer.
 * Format: (NAME: VALUE1NAME~VALUE1VALUE,VALUE2NAME~VALUE2VALUE)
 */
void OutputSocketManager::SendMap(std::string name, StringMap map) {
	std::string message = "(" + name + ":";
	std::vector<std::string> names = map.GetNames();
	
	for(int i=0; i<names.size(); i++) {
		std::string name = names[i];
		std::string value = map.Get(name);
		std::string segment = name + "~" + value;
		message += segment;
		
		if(i < names.size() - 1) {
			message += ",";
		}
	}
	
	message += ")";
	SendMessage(message);
}


void OutputSocketManager::SetDestinationAddress(std::string newAddress) {
	this->socket.Close();
	this->socket = DatagramSocket(newAddress, Constants::OUTPUT_PORT);
}


bool OutputSocketManager::Connected() {
	return this->socket.Connected();
}


bool OutputSocketManager::Bound() {
	return this->socket.Bound();
}
