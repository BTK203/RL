
# Makefile for RobotLib tools.
# Written By: Brach Knutson

CXX = g++

#util
bin/StringUtils.o: util/StringUtils.cpp
	$(CXX) -c -o bin/StringUtils.o util/StringUtils.cpp

bin/XMLDocument.o: util/XMLDocument.cpp
	$(CXX) -c -o bin/XMLDocument.o util/XMLDocument.cpp
	
bin/XMLTag.o: util/XMLTag.cpp
	$(CXX) -c -o bin/XMLTag.o util/XMLTag.cpp
	
bin/XMLTagAttribute.o: util/XMLTagAttribute.cpp
	$(CXX) -c -o bin/XMLTagAttribute.o util/XMLTagAttribute.cpp
	

#utility dependencies for GenerateProjectMakefile	
lib/GenerateProjectMakefileDependencies.a: bin/StringUtils.o bin/XMLDocument.o bin/XMLTag.o bin/XMLTagAttribute.o
	ar rs lib/GenerateProjectMakefileDependencies.a bin/StringUtils.o bin/XMLDocument.o bin/XMLTag.o bin/XMLTagAttribute.o

#root
GenerateProjectMakefile: GenerateProjectMakefile.cpp lib/GenerateProjectMakefileDependencies.a
	$(CXX) -o GenerateProjectMakefile GenerateProjectMakefile.cpp lib/GenerateProjectMakefileDependencies.a
	
all: GenerateProjectMakefile

setup:
	mkdir bin
	mkdir bin/util
	mkdir lib

clean:
	rm -r bin
	rm -r lib
	make setup
	rm GenerateProjectMakefile
