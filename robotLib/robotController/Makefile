
# Makefile for the RobotController library.
# Written By: Brach Knutson

CXX = g++

#util
bin/util/Pin.o: util/Pin.cpp
	$(CXX) -c -o bin/util/Pin.o util/Pin.cpp

bin/util/Clock.o: util/Clock.cpp
	$(CXX) -c -o bin/util/Clock.o util/Clock.cpp
	
bin/util/Socket.o: util/Socket.cpp
	$(CXX) -c -o bin/util/Socket.o util/Socket.cpp

bin/util/DatagramSocket.o: util/DatagramSocket.cpp
	$(CXX) -c -o bin/util/DatagramSocket.o util/DatagramSocket.cpp

bin/util/Util.o: util/Util.cpp
	$(CXX) -c -o bin/util/Util.o util/Util.cpp

allUtil: bin/util/Pin.o bin/util/Clock.o bin/util/Socket.o bin/util/DatagramSocket.o bin/util/Util.o
	
#comm
bin/comm/ButtonPair.o: comm/ButtonPair.cpp
	$(CXX) -c -o bin/comm/ButtonPair.o comm/ButtonPair.cpp
	
bin/comm/ButtonMap.o: comm/ButtonMap.cpp
	$(CXX) -c -o bin/comm/ButtonMap.o comm/ButtonMap.cpp
	
bin/comm/StringPair.o: comm/StringPair.cpp
	$(CXX) -c -o bin/comm/StringPair.o comm/StringPair.cpp
	
bin/comm/StringMap.o: comm/StringMap.cpp
	$(CXX) -c -o bin/comm/StringMap.o comm/StringMap.cpp

bin/comm/InputSocketManager.o: comm/InputSocketManager.cpp
	$(CXX) -c -o bin/comm/InputSocketManager.o comm/InputSocketManager.cpp
	
bin/comm/OutputSocketManager.o: comm/OutputSocketManager.cpp
	$(CXX) -c -o bin/comm/OutputSocketManager.o comm/OutputSocketManager.cpp
	
bin/comm/VideoSocketManager.o: comm/VideoSocketManager.cpp
	$(CXX) -c -o bin/comm/VideoSocketManager.o comm/VideoSocketManager.cpp
	
bin/comm/SecureSocketManager.o: comm/SecureSocketManager.cpp
	$(CXX) -c -o bin/comm/SecureSocketManager.o comm/SecureSocketManager.cpp
	
allComm: bin/comm/ButtonPair.o bin/comm/ButtonMap.o bin/comm/StringPair.o bin/comm/StringMap.o bin/comm/InputSocketManager.o bin/comm/OutputSocketManager.o bin/comm/VideoSocketManager.o bin/comm/SecureSocketManager.o
	
#root
bin/Constants.o: Constants.cpp
	$(CXX) -c -o bin/Constants.o Constants.cpp

bin/RobotController.o: RobotController.cpp
	$(CXX) -c -o bin/RobotController.o RobotController.cpp
	
allRoot: bin/RobotController.o bin/Constants.o

allRobotController: allRoot allComm allUtil

lib/RobotController.a: allRobotController
	ar rs lib/RobotController.a `find bin/*.o`

#utils
setup:
	mkdir bin
	mkdir lib
	mkdir bin/util
	mkdir bin/comm

clean:
	rm -r bin
	rm -r lib
	make setup
	rm Test
