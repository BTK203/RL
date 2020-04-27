#include "util/Util.h"

/**
 * Source file for the GenerateProjectMakefile executable.
 * This executable takes a list of c++ files that are part of a
 * RobotLib project and generates a makefile that compiles them 
 * and links them together, placing it in ../
 * Written By: Brach Knutson
 */
 
static const std::string 
	DEFAULT_MAKEFILE_TEMPLATE_NAME  = "tools/templates/makefileTemplate.txt",
	DEFAULT_PREFERENCES_NAME        = "tools/preferences.xml",
	DEFAULT_OUTPUT_DESTINATION      = "Makefile";
	
std::string 
	makefileDestination      = DEFAULT_OUTPUT_DESTINATION,
	preferencesLocation      = DEFAULT_PREFERENCES_NAME,
	makefileTemplateLocation = DEFAULT_MAKEFILE_TEMPLATE_NAME;


/**
 * UTILITY METHODS FOR DEBUGGIN
 */
void printVector(std::vector<std::string> vec) {
	for(int i=0; i<vec.size(); i++) {
		std::cout << vec[i] << std::endl;
	}
}


void showHelp() {
	std::cout << "GenerateProjectMakefile\n";
	std::cout << "Usage: ./GenerateProjectMakefile [-h] [FILES]\n";
	std::cout << "\n";
	std::cout << "The GenerateProjectMakefile utility is written with the purpose\n";
	std::cout << "of generating a Makefile for compiling a RobotLib projects.\n";
	std::cout << "It works by comparing the input FILES with previously recorded files\n";
	std::cout << "found in files.txt, and then generating a makefile which compiles all files.\n";
	std::cout << "\n";
	std::cout << "OPTIONS:\n";
	std::cout << "-h : Show this help message\n";
	std::cout << std::endl;
}


std::string vectorToString(std::vector<std::string> vec) {
	std::string str = "";
	for(int i=0; i<vec.size(); i++) {
		str += vec[i];
		if(i < vec.size() - 1) {
			str += "\n";
		}
	}
	
	return str;
}


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


std::string readRules(XMLDocument preferences) {
	XMLTag depsTag = preferences.GetTagsByName("RobotLibProject")[0].GetTagsByName("dependencies")[0];
	std::vector<XMLTag> depsVector = depsTag.GetTagsByName("dependency");
	std::string rulesString = "";
	for(int i=0; i<depsVector.size(); i++) {
		XMLTag depRule = depsVector[i].GetTagsByName("rule")[0];
		rulesString += depRule.Content();
		
		if(i < depsVector.size() - 1) {
			rulesString += " ";
		}
	}
	
	return rulesString;
}


std::string readCommands(XMLDocument preferences) {
	XMLTag depsTag = preferences.GetTagsByName("RobotLibProject")[0].GetTagsByName("dependencies")[0];
	std::vector<XMLTag> depsVector = depsTag.GetTagsByName("dependency");
	std::string commandsString = "";
	for(int i=0; i<depsVector.size(); i++) {
		XMLTag depCommand = depsVector[i].GetTagsByName("command")[0];
		commandsString += depCommand.Content();
		
		if(i < depsVector.size() - 1) {
			commandsString += " ";
		}
	}
	
	return commandsString;
}


void writeFile(std::string name, std::string contents) {	
	std::ofstream file = std::ofstream(name.c_str(), std::ofstream::out);
    file.write(contents.c_str(), contents.length());

    file.close();
}


int main(int argc, char *argv[]) {
	std::string 
		makefileDestination      = DEFAULT_OUTPUT_DESTINATION,
		makefileTemplateLocation = DEFAULT_MAKEFILE_TEMPLATE_NAME;
	
	//perform some assertions before we can do stuff
	
	//Make sure we actually have input files to include in the Makefile.
	if(argc <= 1) {
		std::cout << "ERROR: Unable to decide if I need to generate the Makefile! I was given no input files.\n";
		std::cout << "\n";
		showHelp();
		return 1;
	}
	
	//Make sure we set our destination location correctly. If this program operates in its own directory without this assertion it would overwrite its own Makefile.
	std::string currentDirectory = "";
	char *currentDirChar = getenv("PWD");
	if(currentDirChar == NULL) {
		std::cout << "ERROR: The PWD environment variable could not be found." << std::endl;
		return 1;
	} else {
		currentDirectory = std::string(currentDirChar);
	}
	
	//check for /tools, indicating that we are in this files directory and will overwrite our makefile
	int lastForwardSlash = currentDirectory.find_last_of("/");
	if(lastForwardSlash == std::string::npos) {
		std::cout << "ERROR: Could not parse directory: could not find \"/\"!" << std::endl;
		return 1;
	}
	
	std::string mostRecentDirectory = currentDirectory.substr(lastForwardSlash + 1);
	if(mostRecentDirectory == "tools") {
		std::cout << "NOTE: Operating in my own directory, placing Makefile in ../ to avoid overwriting mine." << std::endl;
		makefileDestination = "../" + DEFAULT_OUTPUT_DESTINATION;
		
		//change file destinations to avoid duplicates and things
		preferencesLocation      = "../" + DEFAULT_PREFERENCES_NAME;
		makefileTemplateLocation = "../" + DEFAULT_MAKEFILE_TEMPLATE_NAME;
	}
	
	std::cout << "Reading input..." << std::endl;

	//read all files to be compiled into std::vector inputLines
	std::vector<std::string> inputLines;
	for(int i=1; i<argc; i++) {
		std::string fileName = std::string(argv[i]);
		if(fileName == "-h") {
			showHelp();
			return 0;
		}
		
		inputLines.push_back(fileName);
	}
	
	std::cout << "Reading preferences..." << std::endl;
	XMLDocument preferences = XMLDocument(preferencesLocation);
		
	//generate the makefile!
	std::cout << "Generating Makefile..." << std::endl;
	
	//initalize makefileContents to header defined in templates/makefileTemplate.txt
	std::string makefileContents = vectorToString(readRawFile(makefileTemplateLocation));
	
	//other makefile components
	std::string archiveRuleHeader = "allProject: ";
			
	//now add the rules for compiling each individual file
	for(int i=0; i<inputLines.size(); i++) {
		std::string sourceName = inputLines[i];
		
		//find the location of the extension
		int extensionLocation = sourceName.find(".cpp");
		if(extensionLocation == std::string::npos) { //make sure there is one
			std::cout << "WARNING: Source file " << sourceName << " is an invalid file type and will not be compiled." << std::endl;
		}
		
		//cut off extension and path to get just the file name, then add it to the archive dependencies
		int lastSlash = sourceName.find_last_of("/");
		if(lastSlash == std::string::npos) {
			std::cout << "Something went wrong! I couldn't find any \"/\"'s in " << sourceName << "!" << std::endl;
			lastSlash = 0;
		}
		
		std::string baseName = sourceName.substr(lastSlash + 1, extensionLocation);
		std::string ruleName = "bin/" + baseName + ".o";
		
		//add rule name to rules that need it
		archiveRuleHeader += ruleName + " ";
		
		//Generate rule
		//RULE TEMPLATE:
		//basename.o: sourcename
		//    $(CXX) -c -o basename.o sourcename
		
		std::string rule  = ruleName + ": " + sourceName + " ; $(CXX) -c -o " + ruleName + " " + sourceName + "\n";					
		makefileContents += rule;
	}
		
	makefileContents += "\n";
	makefileContents += archiveRuleHeader += "\n";
	makefileContents += "\n";
		
	//read the dependencies and options from preferences and insert them into the rules
	std::string rules    = readRules(preferences);
	std::string commands = readCommands(preferences);
		
	//the big rule that generates the main executable
	makefileContents += "Main: allProject " + rules + "\n";
	makefileContents += "\t$(CXX) -o Main `find bin/*.o` " + commands + "\n";
	makefileContents += "\n";
	
	writeFile(makefileDestination, makefileContents);
	
	std::cout << "Done!" << std::endl;
	
	return 0;
}
