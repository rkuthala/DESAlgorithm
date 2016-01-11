/*
 * KeyNotifier.cpp
 */
#include <unistd.h> // write
#include "KeyNotifier.h"

KeyNotifier::KeyNotifier() :
		keyFound(false) {

}

KeyNotifier::~KeyNotifier() {

}

bool KeyNotifier::isKeyFound() const {
	return keyFound;
}

void KeyNotifier::setKeyFound(bool keyFound) {
	this->keyFound = keyFound; // Might have to lock
	//Notify other process by writing onto socket
	for (std::vector<int>::iterator var = clientSockets.begin(); var != clientSockets.end(); var++) {
		int n = write(var[0], "Key Found", 10);
		if (n < 0)
			std::cout << "ERROR in notifying other machine \n";
	}
}

std::vector<int> KeyNotifier::getSockFd() const {
	return clientSockets;
}

void KeyNotifier::setSockFd(int sockFd) {
//	sockFD = sockFd;
	clientSockets.push_back(sockFd);
}
