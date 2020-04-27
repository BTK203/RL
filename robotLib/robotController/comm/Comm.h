#ifndef COMM_H
#define COMM_H

#include <iostream>
#include <vector>
#include "../util/Util.h"
#include "opencv2/opencv.hpp"

using namespace cv;

enum Button {
	LT,
	RT,
	LB,
	RB,
	LSTICK_X,
	LSTICK_Y,
	RSTICK_X,
	RSTICK_Y,
	DUP,
	DLEFT,
	DDOWN,
	DRIGHT,
	START,
	SELECT,
	A,
	B,
	X,
	Y,
	LSTICK,
	RSTICK
};


class ButtonPair {
	public:
	ButtonPair() {};
	ButtonPair(Button button, double value);
	Button GetButton();
	double GetValue();
	void SetValue(double value);
	
	private:
	Button button;
	double value;
};


class ButtonMap {
	public:
	ButtonMap();
	void Set(Button button, double value);
	double Get(Button button);
	
	private:
	int indexOf(Button button);
	std::vector<ButtonPair> pairs;
};


class StringPair {
	public:
	StringPair() {};
	StringPair(std::string str, std::string value);
	std::string GetName();
	std::string GetValue();
	void SetValue(std::string value);
	
	private:
	std::string
		name,
		value;
};


class StringMap {
	public:
	StringMap();
	void Set(std::string name, std::string value);
	std::string Get(std::string name);
	std::vector<std::string> GetNames();
	long GetLastUpdatedTime();
	
	private:
	int indexOf(std::string str);
	long lastUpdateTime;
	std::vector<StringPair> pairs;
};

/**
 * Watches a DatagramSocket on its own thread.
 */
class SocketManager {
	public:
	virtual void SetDestinationAddress(std::string newAddress) = 0;
	virtual bool Connected() = 0;
	virtual bool Bound() = 0;
};


class InputSocketManager: public SocketManager {
	public:
	InputSocketManager();
	~InputSocketManager();
	std::string GetLatestMessage();
	void SetDestinationAddress(std::string newAddress);
	bool Connected();
	bool Bound();
	void UpdateConstantly();
	void SendMessage(std::string message);
	bool Updated();

	private:
	void Update();
	void ProcessCommand();
	std::string
		latestMessage,
		latestFullMessage;
		
	long lastUpdatedTime;
		
	pthread_t thread;
	DatagramSocket socket;
};


class OutputSocketManager: public SocketManager {
	public:
	OutputSocketManager();
	~OutputSocketManager();
	void SendMessage(std::string message);
	void SendMap(std::string name, StringMap map);
	void SetDestinationAddress(std::string newAddress);
	bool Connected();
	bool Bound();
	
	private:
	DatagramSocket socket;
};


class VideoSocketManager: public SocketManager {
	public:
	VideoSocketManager();
	~VideoSocketManager();
	void Initalize();
	void SendMessage(std::string message);
	void SendVideo(Mat img);
	void SetDestinationAddress(std::string newAddress);
	bool Connected();
	bool Bound();
	bool Initalized();

	
	private:
	bool initalized;
	Socket socket;
	
	std::string destAddress;
	int destPort;
};


class SecureSocketManager: public SocketManager {
	public:
	SecureSocketManager();
	~SecureSocketManager();
	void SendMessage(std::string message);
	std::string GetLatestMessage();
	void SetDestinationAddress(std::string newAddress);
	bool Connected();
	bool Bound();
	bool Updated();
	void UpdateConstantly();
	bool WaitForMessage(std::string msg, int timeout);
	bool ShouldBeEnabled();
	void SetShouldBeEnabled(bool shouldBeEnabled);
	
	private:
	void Update();
	void ProcessCommand();
	
	bool robotEnabled;
	
	long lastUpdateTime;
	
	std::string
		latestMessage,
		latestFullMessage;
		
	pthread_t thread;
	DatagramSocket socket;
};

#endif
