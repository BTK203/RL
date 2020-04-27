#include <iostream>
#include <vector>
#include <fstream>

static const std::string 
	FILE_PATH_NAME = "/etc/hostapd/hostapd.conf",
	FILE_TEMPLATE_NAME = "/home/pi/robotLib/scripts/conffiles/hostapd.conf";


/**
 * Reads a file from the path name 
 */
std::vector<std::string> readRawFile(std::string name) {
	std::fstream file = std::fstream(name);
    std::vector<std::string> fileLines = std::vector<std::string>();

    std::string line;
    while(std::getline(file, line)) {
        fileLines.push_back(line);
    }
    
    file.close();
    
    return fileLines;
}

/**
 * Writes contents to name.
 */
void writeFile(std::string name, std::string contents) {	
	std::ofstream file = std::ofstream(name.c_str(), std::ofstream::out);
    file.write(contents.c_str(), contents.length());

    file.close();
}


int main(int argc, char *argv[]) {
	std::cout << "RL Hostapd setup" << std::endl;

	std::string netName = "";
	std::string netPass = "";

	//attempt to get valid network data
	while(true) {
		//get the network name
		std::cout << "Robot Network Name:";
		std::cin >> netName;
		
		//get the network password
		std::cout << "Robot Network Password (at least 8 characters): ";
		std::cin >> netPass;
		
		//process the information
		bool nameGood = netName.length() > 0;
		bool passGood = netPass.length() >= 8;
		
		if(nameGood && passGood) {
			break;
		}
		
		if(!nameGood) {
			std::cout << "Network name must be defined." << std::endl;
		}
		
		if(!passGood) {
			std::cout << "Network password must be defined." << std::endl;
		}
	}
	
	//generate the file
	std::cout << "Generating File" << std::endl;
	std::vector<std::string> base = readRawFile(FILE_TEMPLATE_NAME);
	
	//add the name and password to the end of the file. 
	//Example ssid and password:
	//ssid=PiAccessPoint
	//wpa_passphrase=walter!!
	base.push_back("ssid=" + netName);
	base.push_back("wpa_passphrase=" + netPass);
	
	std::string fileContents = "";
	for(int i=0; i<base.size(); i++) {
		fileContents += base[i] + "\n";
	}
	
	//place the file
	std::cout << "Placing File" << std::endl;
	writeFile(FILE_PATH_NAME, fileContents);
	
	std::cout << "hostapd setup done!" << std::endl;
	return 0;
}
