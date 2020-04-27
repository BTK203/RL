#include "../robotController/RobotController.h"

/**
 * Robot!
 * 
 * This class is automatically run by the RobotController, which is
 * called into play by Main.cpp. Do not worry about calling any of the
 * generated methods yourself.
 * Feel free to add any methods or variables you would like here.
 * Do NOT delete any of the generated methods ( init(), whileRunning(),
 * and updateVideo() )!
 * If ANY generated methods are deleted, the program will NOT compile.
 */
class Robot {
	public:
	static void init();
	static void whileRunning();
	static void uploadVideo();
	
	private:
	//sample video utils
	static VideoCapture camera;
	static Mat frame;
};
