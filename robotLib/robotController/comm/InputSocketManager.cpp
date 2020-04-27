#include "../RobotController.h"

/**
 * Source file for the InputSocketWatcher class.
 * Written By: Brach Knutson
 */

InputSocketManager::InputSocketManager() {
	this->socket = DatagramSocket(Constants::DEFAULT_DESTINATION, Constants::INPUT_PORT);
	this->lastUpdatedTime = -100000;
	
	std::cout << "Starting ISM Thread...";
	int res = pthread_create(&this->thread, NULL, RobotController::UpdateISMConstantly, (void *) RobotController::NextAvailableThreadID());
	if(res == 0) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "ERROR" << std::endl;
	}
}


InputSocketManager::~InputSocketManager() {
	this->socket.Close();
}


std::string InputSocketManager::GetLatestMessage() {
	
}


void InputSocketManager::SetDestinationAddress(std::string newAddress) {
	this->socket.Close();
	this->socket = DatagramSocket(newAddress, Constants::INPUT_PORT);
	this->lastUpdatedTime = RobotController::GetClockTime();
}


bool InputSocketManager::Connected() {
	return this->socket.Connected();
}


bool InputSocketManager::Bound() {
	return this->socket.Bound();
}

/**
 * Watches the socket for new messages and updates the last received message.
 */
void InputSocketManager::UpdateConstantly() {
	while(true) {
		Update();
	}
	
	std::cout << "ISM Thread was interrupted, and will no longer respond until restarted." << std::endl;
}


void InputSocketManager::SendMessage(std::string message) {
	this->socket.Send(message);
}


bool InputSocketManager::Updated() {
	long timeSinceUpdate = RobotController::GetClockTime() - lastUpdatedTime;
	return timeSinceUpdate < 500;
}


void InputSocketManager::Update() {
	this->latestMessage += this->socket.Recieve();
	int lastEndIndex = Util::lastIndexOf(latestMessage, Constants::END_CHARACTER, latestMessage.length() - 1);
	if(lastEndIndex > -1) {
		int lastBeginIndex = Util::lastIndexOf(latestMessage, Constants::BEGIN_CHARACTER, lastEndIndex);
		this->latestFullMessage = Util::substring(latestMessage, lastBeginIndex + 1, lastEndIndex);
		
		ProcessCommand();
		latestMessage = "";
	}
	
	delay(Constants::THREAD_LOOP_DELAY);
}


void InputSocketManager::ProcessCommand() {
	lastUpdatedTime = RobotController::GetClockTime();

	if(Util::stringStartsWith(latestFullMessage, "IN:")) { //updates!		
		//parse new data
		std::string newData = Util::substring(latestFullMessage, 3, latestFullMessage.length());
		
		RobotController::UpdateByString(newData);
	}
}
