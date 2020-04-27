#include "../RobotController.h"

/**
 * Source file for the Socket class.
 * Written By: Brach Knutson
 */


Socket::Socket(std::string dest_ip, int port) {
    Init("192.168.0.24", dest_ip, port);
}


bool Socket::Bound() {
    return bound;
}


bool Socket::Connected() {
    return connected;
}


void Socket::Send(std::string msg) {
    const char *bytes = msg.c_str();
    SendBytes(bytes, strlen(bytes));
}


void Socket::SendBytes(const char *bytes, int size) {
    int res = send(this->clientSock, bytes, size, MSG_NOSIGNAL);
}


std::string Socket::Receive() {
    char buffer[1024] = {0};
    int read_result = recv(this->clientSock, buffer, 1024, MSG_DONTWAIT);
    std::string bufferStr = std::string(buffer);
    return bufferStr;
}


void Socket::Close() {
    close(this->ourSock);
    close(this->clientSock);
}


std::string Socket::GetAddress() {
    return this->addrString;
}


int Socket::GetPort() {
    return this->port;
}

/**
 * Waits to recieve msg from the DatagramSocket, but returns after timeout.
 */
bool Socket::WaitForMessage(std::string msg, int timeout) {
    Send(msg);
    
    long startTime = RobotController::GetClockTime();
    long currentTime = RobotController::GetClockTime();
    std::string message = "";
    while(currentTime < (startTime + timeout)) {
        message += Receive();
        if(message.find(msg) != std::string::npos) {
            return true;
        }
        
        delay(1);
        currentTime = RobotController::GetClockTime();
    }
    
    return false;
}


void Socket::Init(std::string this_ip, std::string dest_ip, int port) {
    std::cout << "Initalizing Socket..." << std::endl;
    this->addrString = dest_ip;
    this->port = port;
    
    this->bound = false;
    this->connected = false;
    
    //set up our socket
    ourSock = socket(AF_INET, SOCK_STREAM, 0);
    if(ourSock < 0) {
	Util::err("SOCKET FAILED");
    }
    
    //set up our address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    int option = 1;
    int optSuccess = setsockopt(ourSock, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char*) &option, sizeof(option));
    if(optSuccess < 0) {
	Util::err("SETSOCKOPT FAILED");
    }
    
    //bind the address for a hot second
    int bindSuccess = bind(ourSock, (sockaddr *) &address, sizeof(address));
    this->bound = (bindSuccess >= 0);
    if(!this->bound) {
	Util::err("BIND FAILED");
    }
    
    //listen for incoming connections
    int listenSuccess = listen(ourSock, 3);
    if(listenSuccess < 0) {
	Util::err("LISTEN FAILED");
    }
	
    socklen_t addrSize = sizeof(address);
    //after listening, accept the request we got
    clientSock = accept(ourSock, (sockaddr *) &address, (socklen_t *) &addrSize);
    if(clientSock < 0) {
	Util::err("ACCEPT FAILED");
    }
	
    connected = true;
    std::cout << "TCP Socket Initalized." << std::endl;
}
