/*
 * KeyNotifier.h
 */
#include <cstring>
#include <iostream>
#include <vector>

#ifndef KEYINFO_H_
#define KEYINFO_H_

class KeyNotifier {
private:
	bool keyFound;
	std::string key;

	std::vector<int> clientSockets;
	int sockFD; // To notify other machine.
public:
	KeyNotifier();
	virtual ~KeyNotifier();
	bool isKeyFound() const;
	void setKeyFound(bool keyFound);
	std::vector<int> getSockFd() const;
	void setSockFd(int sockFd);
};

#endif /* KEYINFO_H_ */
