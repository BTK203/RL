#include "../RobotController.h"

/**
 * Source file for the SecureSocketManager class.
 * Written By: Brach Knutson
 */


SecureSocketManager::SecureSocketManager() {
	this->robotEnabled = false;
	this->lastUpdateTime = -100000;
	
	std::cout << "Starting SSM Thread...";
	int res = pthread_create(&this->thread, NULL, RobotController::UpdateSSMConstantly, (void *) RobotController::NextAvailableThreadID());
	if(res == 0) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "ERROR" << std::endl;
	}
}


SecureSocketManager::~SecureSocketManager() {
	this->socket.Close();
}


void SecureSocketManager::SendMessage(std::string message) {
	this->socket.Send(message);
}


std::string SecureSocketManager::GetLatestMessage() {
	return "";
}


void SecureSocketManager::SetDestinationAddress(std::string newAddress) {
	this->socket.Close();
	this->socket = DatagramSocket(newAddress, Constants::SECURE_PORT);
}


bool SecureSocketManager::Connected() {
	return this->socket.Connected();
}


bool SecureSocketManager::Bound() {
	return this->socket.Bound();
}


bool SecureSocketManager::Updated() {
	return (RobotController::GetClockTime() - lastUpdateTime) < 500;
}


void SecureSocketManager::UpdateConstantly() {
	this->socket = DatagramSocket(Constants::DEFAULT_DESTINATION, Constants::SECURE_PORT);
		
	while(true) {
		Update();
	}
	
	std::cout << "SSM Thread was interrupted, and will no longer respond until restarted." << std::endl;
}


void SecureSocketManager::Update() {
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


bool SecureSocketManager::WaitForMessage(std::string msg, int timeout) {
	return this->socket.WaitForMessage(msg, timeout);
}


bool SecureSocketManager::ShouldBeEnabled() {
	return this->robotEnabled;
}


void SecureSocketManager::SetShouldBeEnabled(bool shouldBeEnabled) {
	this->robotEnabled = shouldBeEnabled;
	
	if(RobotController::GetEnabled() != shouldBeEnabled) {
		std::cout << (shouldBeEnabled ? "ENABLED" : "DISABLED") << " at " << RobotController::GetClockTime() << std::endl;
	}
}


void SecureSocketManager::ProcessCommand() {
	lastUpdateTime = RobotController::GetClockTime();
	
	if(Util::stringStartsWith(latestFullMessage, "DISCOV")) {
		std::string newAddress = Util::substring(latestFullMessage, 7, latestFullMessage.length());
		RobotController::ConnectToClient(newAddress);
	}
	
	if(latestFullMessage == "VERIFY") {
		RobotController::SendVerificationSignals();
	}
	
	if(latestFullMessage == "ENABLE") {
		SetShouldBeEnabled(true);
	}
	
	if(latestFullMessage == "DISABLE") {
		SetShouldBeEnabled(false);
	}
	
	
}
