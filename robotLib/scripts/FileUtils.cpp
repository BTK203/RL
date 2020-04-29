#include <vector>
#include <fstream>

/**
 * C++ utils for reading files.
 */

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
