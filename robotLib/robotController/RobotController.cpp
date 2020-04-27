#include "../project/Robot.h"

/**
 * Source file for the RobotController class.
 * Written By: Brach Knutson
 */
 
pthread_t RobotController::mainThread;

Pin RobotController::programPin;
Pin RobotController::radioPin;
Pin RobotController::statusPin;

bool RobotController::initalized = false;
bool RobotController::updated = false;
bool RobotController::enabled = false;
bool RobotController::settingDestinationAddress = false;

Clock RobotController::clock;

std::string RobotController::currentDestination;

int RobotController::nextAvailableThreadID;

long RobotController::lastOutputUpdateTime;

//socket managers
InputSocketManager  RobotController::inputManager;
OutputSocketManager RobotController::outputManager;
VideoSocketManager  RobotController::videoManager;
SecureSocketManager RobotController::secureManager;

//structures containing data values to communicate
ButtonMap RobotController::buttonMap;
StringMap RobotController::strings;
StringMap RobotController::doubles;

const int RobotController::NUM_BUTTONS = 20;
const Button RobotController::BUTTONS[20] = {
	Button::LT,
	Button::RT,
	Button::LB,
	Button::RB,
	Button::LSTICK_X,
	Button::LSTICK_Y,
	Button::RSTICK_X,
	Button::RSTICK_Y,
	Button::DUP,
	Button::DLEFT,
	Button::DDOWN,
	Button::DRIGHT,
	Button::START,
	Button::SELECT,
	Button::A,
	Button::B,
	Button::X,
	Button::Y,
	Button::LSTICK,
	Button::RSTICK
};


/**
 * Inits and starts the RobotController.
 */
void RobotController::Init(int programPin, int radioPin, int statusPin) {
	//set up the wiringPi library so we can talk to the pins
	wiringPiSetup();
	
	currentDestination = Constants::DEFAULT_DESTINATION;
	
	RobotController::programPin = Pin(programPin);
	RobotController::radioPin = Pin(radioPin);
	RobotController::statusPin = Pin(statusPin);
	
	RobotController::clock.Start();
	
	RobotController::initalized = true;
		
	//just for fun
	RobotController::programPin.SetState(PinState::OFF);
	RobotController::radioPin.SetState(PinState::OFF);
	RobotController::statusPin.SetState(PinState::OFF);
	
	RobotController::programPin.Update();
	RobotController::radioPin.Update();
	RobotController::statusPin.Update();
	
	//create the thread and start multitasking
	std::cout << "Starting Main Thread...";
	int res = pthread_create(&RobotController::mainThread, NULL, UpdateConstantly, (void *) NextAvailableThreadID());
	if(res == 0) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "ERROR" << std::endl;
	}
	
	//update the user program and stuff
	Robot::init();
	
	std::cout << "Starting Video Thread...";
	pthread_t videoThread;
	int res2 = pthread_create(&videoThread, NULL, RobotController::UpdateUserVideoStream, (void *) NextAvailableThreadID());
	if(res2 == 0) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "ERROR" << std::endl;
	}
	
	//read out the socket states
	std::cout << "\n";
	std::cout << "Socket Report:\n";
	std::cout << "Input:  " << (RobotController::inputManager.Bound() ? "Bound" : "Unbound") << "\n";
	std::cout << "Output: " << (RobotController::outputManager.Bound() ? "Bound" : " Unbound") << "\n";
	std::cout << "Video:  " << (RobotController::videoManager.Bound() ? "Bound" : "Unbound") << "\n";
	std::cout << "Secure: " << (RobotController::secureManager.Bound() ? "Bound" : "Unbound") << "\n" << std::endl;
	
	while(true) {
		Robot::whileRunning();
	}
}


long RobotController::GetClockTime() {
	return clock.getTimeSinceStart();
}


bool RobotController::GetEnabled() {
	return enabled;
}


double RobotController::GetButton(Button button) {
	if(!enabled) {
		return 0;
	} else {
		return buttonMap.Get(button);
	}
}


void RobotController::PutString(std::string name, std::string value) {
	strings.Set(name, value);
}


void RobotController::PutDouble(std::string name, double value) {
	double roundedValue = Util::roundTo(value, 3);
	doubles.Set(name, std::to_string(value));
}


void RobotController::PutVideo(Mat img) {
	videoManager.SendVideo(img);
}


bool RobotController::isControllerConnected() {
	return updated;
}


int RobotController::NextAvailableThreadID() {
	int next = nextAvailableThreadID;
	nextAvailableThreadID++;
	return next;
}


void *RobotController::UpdateISMConstantly(void *id) {
	inputManager.UpdateConstantly();
}


void *RobotController::UpdateSSMConstantly(void *id) {
	secureManager.UpdateConstantly();
}


void RobotController::UpdateByString(std::string input) {
	int commas = Util::instancesInString(input, ',');
	std::string segments[commas + 1];
		
	Util::splitString(input, ',', segments);
	
	//store the values in the ButtonMap. The values are presented in the exact order as in BUTTONS.
	if((commas + 1) == NUM_BUTTONS) {
		for(int i=0; i<NUM_BUTTONS; i++) {
			buttonMap.Set(BUTTONS[i], std::stod(segments[i]));
		}
	} else {
		std::cout << "WARNING: INPUT STRING IMPROPERLY FORMATTED!" << std::endl;
	}
}


void RobotController::SetNewDestinationAddress(std::string address) {
	if(!settingDestinationAddress) {
		settingDestinationAddress = true;
		if(currentDestination == "192.168.0.255" || address == "192.168.0.255") {
			currentDestination = address;
			
			inputManager.SetDestinationAddress(address);
			outputManager.SetDestinationAddress(address);
			videoManager.SetDestinationAddress(address);
			secureManager.SetDestinationAddress(address);
			
			std::cout << "Setting destination address to \"" << address << "\"" << std::endl;
		} 
		
		settingDestinationAddress = false;
	}
}

/**
 * Sends verification signals to the controller to verify that all sockets are functional.
 * NOTE: this method does NOT send a signal to the secure socket, as the verification
 * process is triggered by signals going through the secure socket. Therefore, if the 
 * verification process is started, it can be safely assumed that the secure socket is
 * functional.
 */
void RobotController::SendVerificationSignals() {
	inputManager.SendMessage("(VERIFY)");
	outputManager.SendMessage("(VERIFY)");
	videoManager.SendMessage("(VERIFY)");
	
	bool success = secureManager.WaitForMessage("(OK)", 1000);
	if(!success) {
		std::cout << "VERIFICATION RESPONSE NOT RECEIVED. POSSIBLE LOS" << std::endl;
	}
}


void RobotController::ConnectToClient(std::string address) {
	if(!isControllerConnected()) {
		RobotController::SetNewDestinationAddress(address);
		//attempt to connect the video stream tool to the client
		pthread_t videoConnectThread;
		int result = pthread_create(&videoConnectThread, NULL, AttemptToConnectVideoStream, (void *) NextAvailableThreadID());
		if(result == 0) {
			delay(50); //give socket some time to do its thing
			//tell the client to connect to us
			secureManager.SendMessage("(CONVID)");
			long threadStartTime = GetClockTime();
			long currentTime = threadStartTime;
			long destinationTime = threadStartTime + 1000; //wait 1 second for video stream to connect
			while(currentTime < destinationTime) {
				if(videoManager.Initalized()) {
					std::cout << "Video Stream Connected." << std::endl;
					break;
				}
				
				currentTime = GetClockTime();
			}
		} else {
			std::cout << "ERROR: Failed to start video init thread!" << std::endl;
		}
			
		secureManager.SendMessage("(CONNECT)");
	} else {
		std::cout << "Refused Request at " << GetClockTime() << " to keep the current connection." << std::endl;
	}
}


void RobotController::Update() {
	clock.Iterate();
	
	//update output
	if(GetClockTime() - lastOutputUpdateTime > 25) {
		outputManager.SendMap("STRINGS", strings);
		outputManager.SendMap("DOUBLES", doubles);
		lastOutputUpdateTime = GetClockTime();
	}
	
	//update Program pin state
	programPin.SetState(PinState::STEADY); //program pin on since the program is running
	
	//update radio pin state
	bool socketsBound = SocketsBound();
	bool socketsConnected = SocketsConnected();
	
	if(!(socketsBound || socketsConnected)) {
		radioPin.SetState(PinState::OFF);
	}
	
	if(socketsBound && !socketsConnected) {
		radioPin.SetState(PinState::BLINK);
		radioPin.SetBlinkRate(500);
	}
	
	bool socketsGoodToGo = socketsBound && socketsConnected;
	if(socketsGoodToGo) {
		radioPin.SetState(PinState::STEADY);
	}
	
	//update enable pin state
	enabled = secureManager.ShouldBeEnabled(); //update enabled state
	if(enabled) {
		statusPin.SetBlinkRate(250);
		statusPin.SetState(PinState::BLINK);
	} else if(socketsGoodToGo) {
		statusPin.SetState(PinState::STEADY);
	} else {
		statusPin.SetState(PinState::OFF);
	}
	
	programPin.Update();
	radioPin.Update();
	statusPin.Update();
	
	updated = inputManager.Updated();
	
	// put ourselves into "discovery mode" if there is an LOS
	if(currentDestination != "192.168.0.255" && !(updated || secureManager.Updated())) {
		std::cout << "LOS at " << GetClockTime() << ". Going into discovery mode." << std::endl;
		secureManager.SetShouldBeEnabled(false);
		SetNewDestinationAddress("192.168.0.255");
	}
}


void *RobotController::UpdateConstantly(void *threadID) {	
	while(true) {
		Update();
	}
}


bool RobotController::SocketsBound() {
	return 
		inputManager.Bound() &&
		outputManager.Bound() &&
		secureManager.Bound();
}


bool RobotController::SocketsConnected() {
	return
		inputManager.Connected() &&
		outputManager.Connected() &&
		secureManager.Connected();
}


void *RobotController::UpdateUserVideoStream(void *id) {
	while(true) {
		Robot::uploadVideo();
	}
}


void *RobotController::AttemptToConnectVideoStream(void *id) {
	videoManager.Initalize();
}
