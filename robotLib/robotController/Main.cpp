#include "RobotController.h"

/**
 * Main source file for tests done for the RobotController library.
 * Written By: Brach Knutson
 */
 
using namespace cv;
 
void *handleVideo(void *id) {
	VideoCapture cap = VideoCapture(0);
	Mat img;
	
	while(cap.isOpened()) {
		bool success = cap.read(img);
		if(success) {
			RobotController::PutVideo(img);
		} else {
			std::cout << "IMG GRAB FAILED!" << std::endl;
		}
	}
	
	std::cout << "video handler exiting" << std::endl;
	
}
 

void robotControllerTest() {
	RobotController::Init(8, 9, 7);
	std::cout << "RobotController created." << std::endl;
	softPwmCreate(16, 0, 100);
	
	std::cout << "Starting video thread...";
	pthread_t thread;
	int res = pthread_create(&thread, NULL, handleVideo, (void *) 4);
	if(res == 0) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "ERROR" << std::endl;
	}
	
	while(true) {
		double throttle = RobotController::GetButton(Button::RT) - RobotController::GetButton(Button::LT);
		double steering = RobotController::GetButton(Button::LSTICK_X);
		RobotController::PutDouble("Throttle", throttle * 100);
		RobotController::PutDouble("Steering", steering * 100);
		softPwmWrite(16, (int) (throttle * 100));
	}
}


void stringSplitTest() {
	std::string test = "Hello! I am a string from the std library. I have spaces.";
	std::string result[12];
	int occurances = Util::splitString(test, ' ', result);
	std::cout << "occurrances: " << occurances << std::endl;
	
	std::cout << "RESULTS: " << std::endl;
	for(int i=0; i<occurances; i++) {
		std::cout << "\"" << result[i] << "\"" << std::endl;
	}
}

 
int main(int argc, char *argv[]) {	
	robotControllerTest();
	return 0;
}
