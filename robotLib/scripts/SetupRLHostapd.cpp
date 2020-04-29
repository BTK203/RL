#include <iostream>
#include <vector>
#include <fstream>
#include "stdlib.h"
#include "unistd.h"

static const std::string 
	FILE_PATH_NAME = "/etc/hostapd/hostapd.conf",
	FILE_TEMPLATE_NAME = "scripts/conffiles/hostapd.conf"; //missing RL prefix, but we will address that by grabbing env variable
	
static const int 
	PATH_MAX_SIZE = 256;

extern std::vector<std::string> readRawFile(std::string name);
extern void writeFile(std::string name, std::string contents);

int instancesInString(std::string searchString, char occurance) {
	int counter = 0;
	for(int i=0; i<searchString.length(); i++) {
		if(searchString.at(i) == occurance) {
			counter++;
		}
	}
	
	return counter;
}

/**
 * Splits A string into segments based on a passed divider.
 * @param str the string to split
 * @param divider the divider to split the string by.
 * @param results a pointer to an array to put the results into
 * @return how many segments the string was split into
 */
int splitString(std::string str, char divider, std::string *result) {
	std::string currentBuffer = "";
	int instancesOfDivider = instancesInString(str, divider);
	int currentInstance = 0;
	
	for(int i=0; i<str.length(); i++) {
		char currentChar = str.at(i);
		
		if(currentChar == divider) {
			result[currentInstance] = currentBuffer;
			currentBuffer = "";
			currentInstance++;
		} else {
			currentBuffer += currentChar;
		}
	}
	
	result[currentInstance] = currentBuffer;
	return instancesOfDivider + 1;
}

void showHelp() {
	std::cout << std::endl;
	std::cout << "sudo -E setuprlhostapd [-t] [TEMPLATE_FILE]\n";
	std::cout << "NOTE: This command accesses files in the /etc folder, and MUST be run with sudo.\n";
	std::cout << "\nOptions:\n";
	std::cout << "-t : specify template location, overriding environment variable.\n";
	std::cout << std::endl;
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
		
	std::string fileTemplateLocation = "";
	char *rlDir = getenv("RLDir");
	if(rlDir == NULL) {
		std::cout << "RLDir environment variable not found. Falling back to -t flag." << std::endl;
		
		if(argc > 2 && std::string(argv[1]) == "-t") {
			fileTemplateLocation = std::string(argv[2]);
		} else {
			std::cout << "-t flag either not provided, or template not following.\n";
			std::cout << "FATAL: Template file for hostapd.conf could not be found!\n";
			std::cout << "Ensure that the RLDir environment variable is defined. If not, use \"createrlev --set [location of robotLib directory]\" to set it.\n";
			std::cout << "Also ensure that you are running this command in sudo with the -E flag. (sudo -E setuprlhostapd)" << std::endl;
			showHelp();
			return 1;
		}
	} else {
		fileTemplateLocation = std::string(rlDir) + "/" + FILE_TEMPLATE_NAME;
	}
	
	//generate the file
	std::cout << "Generating File" << std::endl;
	std::vector<std::string> base = readRawFile(fileTemplateLocation);
		
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
