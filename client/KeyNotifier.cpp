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
	int n = write(sockFD, "Key Found", 10);
	if (n < 0)
		std::cout << "ERROR in notifying other machine \n";
}

int KeyNotifier::getSockFd() const {
	return sockFD;
}

void KeyNotifier::setSockFd(int sockFd) {
	sockFD = sockFd;
}
