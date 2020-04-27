#include "../RobotController.h"

/**
 * Source file for the VideoSocketManager class.
 * Written By: Brach Knutson
 */
 
using namespace cv;


VideoSocketManager::VideoSocketManager() {
	this->initalized = false;
	this->destAddress = Constants::DEFAULT_DESTINATION;
	this->destPort = Constants::VIDEO_PORT;
}


VideoSocketManager::~VideoSocketManager() {
	this->socket.Close();
}


void VideoSocketManager::Initalize() {
	if(this->initalized) {
		this->initalized = false;
		this->socket.Close();
	}
	
	this->socket = Socket(destAddress, destPort);
	this->initalized = true;
}


void VideoSocketManager::SendMessage(std::string message) {
	this->socket.Send(message);
}


void VideoSocketManager::SendVideo(Mat img) {
	if(this->initalized) {
		std::vector<uchar> jpeg;
		std::vector<int> params(2);
		params[0] = IMWRITE_JPEG_QUALITY;
		params[1] = 80;
		imencode(".jpg", img, jpeg, params);
		
		const int imageSize = (const int) jpeg.size();
		char jpegArray[imageSize] = {};
		for(int i=0; i<imageSize; i++) {
			jpegArray[i] = (const char) jpeg[i];
		}
				
		this->socket.SendBytes(jpegArray, imageSize);
	}
}


void VideoSocketManager::SetDestinationAddress(std::string newAddress) {
	this->destAddress = newAddress;
	
	if(newAddress == "192.168.0.255") {
		this->socket.Close();
		this->initalized = false;
	}
}


bool VideoSocketManager::Connected() {
	return this->initalized ? this->socket.Connected() : false;
}


bool VideoSocketManager::Bound() {
	return this->initalized ? this->socket.Bound() : false;
}


bool VideoSocketManager::Initalized() {
	return this->initalized;
}
