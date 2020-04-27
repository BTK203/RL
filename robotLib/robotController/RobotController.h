#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <iostream>
#include <pthread.h>
#include <math.h>
#include "util/Util.h"
#include "comm/Comm.h"

using namespace cv;

class Constants {
	public:
	static const int 
		INPUT_PORT,
		OUTPUT_PORT,
		VIDEO_PORT,
		SECURE_PORT,
		
		THREAD_LOOP_DELAY;
		
	static const char
		*DEFAULT_ADDRESS,
		*DEFAULT_DESTINATION;
	
	static const char
		BEGIN_CHARACTER,
		END_CHARACTER;
};

class RobotController {
	public:
	static void Init(int programPin, int radioPin, int statusPin);
	static long GetClockTime();
	static bool GetEnabled();
	static double GetButton(Button button);
	static void PutString(std::string name, std::string value);
	static void PutDouble(std::string name, double value);
	static void PutVideo(Mat img);
	static bool isControllerConnected();
	static int NextAvailableThreadID();
	
	//socket utils
	static void *UpdateISMConstantly(void *id);
	static void *UpdateSSMConstantly(void *id);
	static void UpdateByString(std::string input);
	static void SetNewDestinationAddress(std::string address);
	static void SendVerificationSignals();
	static void ConnectToClient(std::string address);
	
	static const int NUM_BUTTONS;
	static const Button BUTTONS[20];
	
	private:
	static void Update();
	static void *UpdateConstantly(void *threadID);
	
	//utils
	static bool SocketsBound();
	static bool SocketsConnected();
	static void *UpdateUserVideoStream(void *id);
	static void *AttemptToConnectVideoStream(void *id);
	
	static pthread_t
		mainThread;
	
	static Pin
		programPin,
		radioPin,
		statusPin;
	
	static bool
		initalized,
		updated,
		enabled,
		settingDestinationAddress;
		
	static Clock
		clock;
		
	static std::string
		currentDestination;
		
	static int
		nextAvailableThreadID;
		
	static long lastOutputUpdateTime;
	
	static InputSocketManager  inputManager;
	static OutputSocketManager outputManager;
	static VideoSocketManager  videoManager;
	static SecureSocketManager secureManager;
	
	static ButtonMap buttonMap;
	static StringMap strings;
	static StringMap doubles;
};

#endif
