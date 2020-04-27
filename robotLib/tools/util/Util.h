#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <fstream>

class StringUtils {
        public:
        static std::vector<std::string> SplitString(std::string str, char character);
        static std::string Substring(std::string str, int begin, int end);
        static bool StringStartsWith(std::string str, std::string startsWith);
        static int CountCharacters(std::string str, char character);
};

class XMLTagAttribute {
        public:
        XMLTagAttribute(std::string name, std::string value);
        std::string Name() { return this->name; };
        std::string Value() { return this->value; };
        std::string ReturnString();

        private:
        std::string name;
        std::string value;
};

/**
* Represents a tag formatted in XML that may be found as a child of others (or contain others)
*/
class XMLTag {
        public:
        XMLTag() {};
        XMLTag(std::string name);
        XMLTag(std::string name, std::vector<XMLTagAttribute> attributes);
        XMLTag(std::string name, std::vector<XMLTag> children);
        XMLTag(std::string name, std::vector<XMLTag> children, std::vector<XMLTagAttribute> attributes);
        XMLTag(std::string name, std::string content, std::vector<XMLTagAttribute> attributes);
        XMLTag(std::string name, std::string content);
        void AddTag(XMLTag tag);
        void AddAttribute(XMLTagAttribute attribute);
        void AddContent(std::string content);
        std::vector<XMLTag> GetTagsByName(std::string name);
        std::vector<XMLTag> Tags() { return this->children; };
        std::vector<XMLTagAttribute> GetAttributesByName(std::string name);
        std::vector<XMLTagAttribute> Attributes() { return this->attributes; };
        std::string Name() { return this->name; };
        std::string Content() { return this->content; };
        std::string ReturnString(std::string prefix);
        std::string ReturnString();

        private:
        bool containsChildren;
        std::string name;
        std::string content;
        std::vector<XMLTag> children;
        std::vector<XMLTagAttribute> attributes;
};

/**
* Represents a text document formatted in XML
*/
class XMLDocument {
        public:
        XMLDocument();
        XMLDocument(std::vector<XMLTag> tags);
        XMLDocument(std::string filePath);
        bool HasContents() { return this->hasContents; };
        void AddTag(XMLTag tag);
        std::vector<XMLTag> GetTagsByName(std::string name);
        std::string FileName() { return this->fileName; };
        std::string ReturnString();
        void WriteFile(std::string filePath);

        private:
        std::string fileName;
        bool hasContents;
        std::vector<XMLTag> children;
};

#endif
