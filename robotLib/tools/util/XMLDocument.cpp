#include "Util.h"

/**
 * Source file for the XMLDocument class.
 * Written By: Brach Knutson
 */

/**
 * Creates a blank XML document.
 */
XMLDocument::XMLDocument() {
    this->fileName = "";
    this->hasContents = false;
}

/**
 * Creates an XML document starting with the given tags.
 */
XMLDocument::XMLDocument(std::vector<XMLTag> tags) {
    this->fileName = "";
    this->children = tags;

    if(tags.size() > 0) {
        this->hasContents = true;
    } else {
        this->hasContents = false;
    }
}

/**
 * Creates a new XML document using the formatted file passed.
 */
XMLDocument::XMLDocument(std::string fileName) {
    this->fileName = fileName;
    std::fstream log = std::fstream(fileName);
    std::vector<std::string> fileLines = std::vector<std::string>();

    std::string line;
    while(std::getline(log, line)) {
        //get rid of spaces at beginning of each line
        int beginningOfContent = 0;
        std::string trimmedContent = "";
        for(int i=0; i<line.length(); i++) {
            if(line.at(i) != ' ' && line.at(i) != '\t') {
                beginningOfContent = i;
                break;
            }
        }

        for(int k=beginningOfContent; k<line.length(); k++) {
            trimmedContent += line.at(k);
        }
        
        fileLines.push_back(trimmedContent);
    }

    //if no lines, return because theres no point anyway
    if(fileLines.size() < 1) {
        std::cout << "XMLDocument: read empty file!" << std::endl;
        this->hasContents = false;
        return;
    }

    //parse the vector of lines and build a hierarchy of xml tags for this document
    std::vector<XMLTag> workingHierarchy = std::vector<XMLTag>(); //the hierarchy of tags to be completed
    for(int i=0; i<fileLines.size(); i++) {
        std::string line = fileLines[i];
        if(StringUtils::StringStartsWith(line, "<") && !StringUtils::StringStartsWith(line, "<!--")) {
            int closingBracket = line.find('>');
            std::string tag = StringUtils::Substring(line, 1, closingBracket);
            std::string content = StringUtils::Substring(line, closingBracket + 1, line.length());
            
            if(StringUtils::StringStartsWith(tag, "/")) {
                //ending tag, rebuild the hierarchy without this tag
                
                if(workingHierarchy.size() > 1) {
                    XMLTag addTag = workingHierarchy[workingHierarchy.size() - 1];
                    workingHierarchy[workingHierarchy.size() - 2].AddTag(addTag);
                } else {
                    this->AddTag(workingHierarchy[0]);
                }
                std::vector<XMLTag> newHierarchy = std::vector<XMLTag>();
                for(int k=0; k<workingHierarchy.size() - 1; k++) {
                    newHierarchy.push_back(workingHierarchy[k]);
                }
                workingHierarchy = newHierarchy;
            } else {
                std::vector<std::string> nameAndAttrs = StringUtils::SplitString(tag, ' ');
                std::vector<XMLTagAttribute> attrs = std::vector<XMLTagAttribute>();
                std::string name = nameAndAttrs[0];

                for(int k=1; k<nameAndAttrs.size(); k++) {
                    std::string attrString = nameAndAttrs[k];
                    std::string attrName = StringUtils::SplitString(attrString, '=')[0];
                    std::string attrValue = StringUtils::SplitString(attrString, '=')[1];

                    //if there are spaces in the value, get rid of them
                    while(StringUtils::CountCharacters(attrValue, '"') < 2) {
                        attrValue += " " + nameAndAttrs[k+1];
                        k++;
                    }

                    //take off quotes 
                    attrValue = StringUtils::Substring(attrValue, 1, attrValue.length()-1);
                    XMLTagAttribute newAttr = XMLTagAttribute(attrName, attrValue);
                    attrs.push_back(newAttr);
                }
                XMLTag newTag = XMLTag(name, attrs);
                if(content.length() > 0) {
                    //get the index of the starting bracket of the closing tag
                    int closingTag = content.find("</" + newTag.Name() + ">");
                    content = StringUtils::Substring(content, 0, closingTag);
                    newTag.AddContent(content);
                    
                    if(workingHierarchy.size() > 0) {
                        workingHierarchy[workingHierarchy.size() - 1].AddTag(newTag);
                    } else {
                        this->AddTag(newTag);
                    }

                } else {
                    workingHierarchy.push_back(newTag);
                }
            }
        } else {
            if(!StringUtils::StringStartsWith(line, "<!--")) {
                //line is not comment, parse as content for the current tag
                workingHierarchy[workingHierarchy.size()-1].AddContent(line);
            }
        }
        
    }

    this->hasContents = true;
}

/**
 * Adds the passed tag to the XML document.
 */
void XMLDocument::AddTag(XMLTag tag) {
    this->children.push_back(tag);
    this->hasContents = true;
}

/**
 * Returns a list of XML tags with the given name.
 */
std::vector<XMLTag> XMLDocument::GetTagsByName(std::string name) {
    std::vector<XMLTag> results = std::vector<XMLTag>();

    for(int i=0; i<this->children.size(); i++) {
        if(this->children[i].Name() == name) {
            results.push_back(this->children[i]);
        }
    }

    if(results.size() == 0) {
        std::cout << "There were no XML Tag results for \"" << name << "\"" << std::endl;
    }

    return results;
}

/**
 * Returns a string formatted in XML representing the current object.
 */
std::string XMLDocument::ReturnString() {
    std::string results = "";

    for(int i=0; i<this->children.size(); i++) {
        results += this->children[i].ReturnString();
        results += "\n";
    }

    return results;
}

/**
 * Writes the XML document to the given file.
 */
void XMLDocument::WriteFile(std::string fileName) {
    std::ofstream file = std::ofstream(fileName.c_str(), std::ofstream::out);
    std::string toWrite = this->ReturnString();
    file.write(toWrite.c_str(), toWrite.length());

    file.close();
}
