#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <cmath>
#include <chrono>
#include <fstream>
#include "wiringPi.h"
#include "softPwm.h"
#include "netdb.h"
#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"

enum PinState {
	OFF,
	STEADY,
	BLINK,
};

/**
 * Utilities for stuff
 */
class Util {
	public:
	static int indexOf(std::string str, char target, int start);
	static int lastIndexOf(std::string str, char target, int start);
	static std::string substring(std::string str, int begin, int end);
	static bool stringStartsWith(std::string str, std::string phrase);
	static int splitString(std::string str, char divider, std::string *result);
	static int instancesInString(std::string searchString, char occurance);
	static double roundTo(double value, int zeros);
	static void err(const char *msg);
};


class Pin {
	public:
	Pin() {};
	Pin(int pinNumber);
	void Update();
	void SetState(PinState state);
	PinState GetState();
	void SetBlinkRate(int rate);
	int GetBlinkRate();
	
	private:
	int
		pinNumber,
		blinkRate;
		
	PinState state;
};

class Clock {
	public:
	Clock();
	void Start();
	void Iterate();
	long getTimeSinceStart();
	
	private:
	long timeSinceEpoch();
	
	long lastTime;
	long counter;
};

/**
 * A TCP Socket sender utility that sends and receives information to and from a client.
 */
class Socket {
	public:
	Socket() {};
	Socket(std::string dest_ip, int port);
	bool Bound();
	bool Connected();
	void Send(std::string msg);
	void SendBytes(const char *bytes, int size);
	std::string Receive();
	void Close();
	std::string GetAddress();
	int GetPort();
	bool WaitForMessage(std::string msg, int timeout);
	
	private:
	void Init(std::string this_ip, std::string dest_ip, int port);
	
	int
		ourSock,
		clientSock;
		
	sockaddr_in address;
	
	bool
		bound,
		connected;
		
	std::string addrString;
	int port;
};

/**
 * A UDP Socket sender utility that sends and recieves information to and from a client.
 */
class DatagramSocket {
	public:
	DatagramSocket(){};
	DatagramSocket(std::string dest_ip, int port);
	DatagramSocket(std::string dest_ip, int port, bool blockUntilConnected);
	bool AttemptToConnect();
	bool AttemptToBind();
	bool Connected() { return this->connected; };
	bool Bound() { return this->bound; };
	void Send(std::string msg);
	void SendBytes(const char *bytes, int size);
	std::string Recieve();
	void Close();
	std::string GetAddress() { return this->address; };
	int GetPort() { return this->port; };
	bool WaitForMessage(std::string msg, int timeout);

	private:
	void Init(std::string this_ip, std::string dest_ip, int port, bool blockUntilConnected);
	
	//variables returned by c socket methods
	int sock; //sock descriptor returned by socket() call
	sockaddr_in our_address; //address of this computer
	sockaddr_in client_address; //address of the server

	//local variables for returning
	bool 
		bound,
		connected;
		
	std::string address;
	int port;
};


#endif
