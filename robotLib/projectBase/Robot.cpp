#include "Robot.h"

/**
 * Source file for the Robot class.
 */
using namespace cv;

VideoCapture Robot::camera = VideoCapture(0);
Mat          Robot::frame  = Mat();

/**
 * Called when the program starts up.
 */
void Robot::init() {
	
}

/**
 * Called while the program is active.
 */
void Robot::whileRunning() {
	
}

/**
 * Called in a separate thread with the purpose of handling video, meaning
 * that this method is completely optional for the user to fill out.
 * Video can be uploaded in the form of an OpenCV Mat object using
 * the RobotController::PutVideo() method.
 * 
 * This method is pre-populated with a bare-bones video stream, which can
 * be modified to add smarter functionality.
 * 
 * NOTE: this method is called periodically, meaning that an OpenCV
 * VideoCapture object should be declared anddefined somewhere outside 
 * of this method.
 */
void Robot::uploadVideo() {
	bool success = camera.read(frame);
	if(success) {
		RobotController::PutVideo(frame);
	} else {
		std::cout << "Unable to grab video frame" << std::endl;
	}
}
