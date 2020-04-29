#include <iostream>
#include <vector>
#include <fstream>

/**
 * C++ script file for setting and removing the RobotLib environment variable.
 * Written By: Brach Knutson
 */

static const std::string
	ENV_FILE = "/etc/environment",
	VAR_NAME = "RLDir";
	
extern std::vector<std::string> readRawFile(std::string name);
extern void writeFile(std::string name, std::string contents);

void showHelp() {
	std::cout << "createrlev: ERROR: Improper usage detected. Action could not be completed." << std::endl;
	std::cout << "Usage: createrlev [--set OR --remove] [VALUE]" << std::endl;
}

bool stringStartsWith(std::string str, std::string phrase) {
	return str.substr(0, phrase.length()) == phrase;
}

int varIndex(std::vector<std::string> fileLines) {
	for(int i=0; i<fileLines.size(); i++) {
		if(stringStartsWith(fileLines[i], VAR_NAME)) {
			return i;
		}
	}
	
	return -1;
}

int main(int argc, char *argv[]) {
	//read env file
	std::vector<std::string> fileLines = readRawFile(ENV_FILE);
	int varLocation = varIndex(fileLines);
		
	if(std::string(argv[1]) == "--set" && argc >= 3) {
		//sets the environment variable
		std::string varValue = argv[2];		
		std::string newLine = VAR_NAME + "=" + varValue;
		
		if(varLocation < 0) { //if variable doesnt exist...
			fileLines.push_back(newLine); //add it to the end of the file
		} else {
			fileLines[varLocation] = newLine;
		}
	} else if(std::string(argv[1]) == "--remove" && argc >= 2) {
		if(varLocation < 0) { //it doesnt exist...
			std::cout << "createrlev: Variable was not found in file, so no variable removed." << std::endl;
			return 1;
		}
		
		//it does exist...
		fileLines.erase(fileLines.begin() + varLocation);
	} else {
		showHelp();
		return 1;
	}
	
	//write the new file
	std::string newFile = "";
	for(int i=0; i<fileLines.size(); i++) {
		newFile += fileLines[i] + "\n";
	}
	writeFile(ENV_FILE, newFile);
	return 0;
}
