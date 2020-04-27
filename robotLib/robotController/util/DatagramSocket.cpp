#include "../RobotController.h"

/**
 * source file for DatagramSocket class
 * Written By: Brach Knutson
 */


/**
 * Construct a new DatagramSocket object connecting to dest_ip on port
 * @param dest_ip a string containing the ip of the server in standard dot notation
 * @param port the port to connect on
 */
DatagramSocket::DatagramSocket(std::string dest_ip, int port) {
    Init(Constants::DEFAULT_ADDRESS, dest_ip, port, false);
}


DatagramSocket::DatagramSocket(std::string dest_ip, int port, bool blockUntilConnected) {
    Init(Constants::DEFAULT_ADDRESS, dest_ip, port, blockUntilConnected);
}

/**
 * Attempts to connect to the router.
 * @return true if the connection was successful, or the DatagramSocket was already connected, false otherwise.
 */
bool DatagramSocket::AttemptToConnect() {
    int connect_result = connect(this->sock, (sockaddr*) &this->client_address, sizeof(this->client_address));
    this->connected = (connect_result > -1);
    return this->connected;
}

/**
 * Attempts to bind to our address
 * @return true if operation was successful, false otherwise.
 */
bool DatagramSocket::AttemptToBind() {
    //"bind" our address to the DatagramSocket
    int bind_result = bind(this->sock, (sockaddr*) &our_address, sizeof(our_address));
    this->bound = !(bind_result < 0);
    return this->bound;
}

/**
 * Sends the given message to the destination
 * @param msg a string containing the message to send
 */
void DatagramSocket::Send(std::string msg) {
    const char *buffer = msg.c_str();
    SendBytes(buffer, strlen(buffer));
    //int send_result = send(this->sock, buffer, strlen(buffer), 0); //the big send
}

/**
 * Sends size amount of bytes.
 */
void DatagramSocket::SendBytes(const char *bytes, int size) {
    int send_result = send(this->sock, bytes, size, 0);
}

/**
 * Reads the buffer and returns whats there
 * @return a string containing the contents of the buffer
 */
std::string DatagramSocket::Recieve() {
    char buffer[1024] = {0};
    int read_result = recv(this->sock, buffer, 1024, MSG_DONTWAIT);
    std::string bufferStr = std::string(buffer);
    return bufferStr;
}

/**
 * closes the DatagramSocket
 */
void DatagramSocket::Close() {
    close(this->sock);
}

/**
 * Waits to recieve msg from the DatagramSocket, but returns after timeout.
 */
bool DatagramSocket::WaitForMessage(std::string msg, int timeout) {
    Send(msg);
    
    long startTime = RobotController::GetClockTime();
    long currentTime = RobotController::GetClockTime();
    std::string message = "";
    while(currentTime < (startTime + timeout)) {
        message += Recieve();
        if(message.find(msg) != std::string::npos) {
            return true;
        }
        
        delay(1);
        currentTime = RobotController::GetClockTime();
    }
    
    return false;
}


void DatagramSocket::Init(std::string this_ip, std::string dest_ip, int port, bool blockUntilConnected) {
	this->address = dest_ip;
    this->port = port;
    
    this->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->sock < 0) //socket creation didnt be like that tho
        Util::err("socket FAILED");

    //define destination address
    memset(&this->client_address, 0, sizeof(this->client_address));
    this->client_address.sin_family = AF_INET;
    this->client_address.sin_port = htons(port);
    
    //define our address
    memset(&our_address, 0, sizeof(our_address));
    our_address.sin_family = AF_INET;
    our_address.sin_port = htons(port);

    //convert "this_ip" to a usable address struct
    int aton_result_1 = inet_aton(this_ip.c_str(), &our_address.sin_addr);
    if(aton_result_1 != 1) {
		std::cout << "this aton result: " << aton_result_1 << std::endl;
        Util::err("THIS ATON FAILED");
	}

    //convert "dest_ip" to a usable address struct
    int pton_result_2 = inet_pton(AF_INET, dest_ip.c_str(), &this->client_address.sin_addr);
    if(pton_result_2 != 1) 
        Util::err("DEST PTON FAILED");
    
    AttemptToBind();

    //wait for the connect to succeed before we go
    while(true) {
        bool connectSuccess = AttemptToConnect();
        if(connectSuccess || !blockUntilConnected) //the connection was the big good or we stopped trying
            break;
    }
}
