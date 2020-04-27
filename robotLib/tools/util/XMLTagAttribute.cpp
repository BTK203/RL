#include "Util.h"

/**
 * Source file for the XMLTagAttribute class.
 */


XMLTagAttribute::XMLTagAttribute(std::string name, std::string value) {
    this->name = name;
    this->value = value;
}


std::string XMLTagAttribute::ReturnString() {
    return name + "=\"" + value + "\"";
}
