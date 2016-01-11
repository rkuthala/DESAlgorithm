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

	int sockFD; // To notify server.
public:
	KeyNotifier();
	virtual ~KeyNotifier();
	bool isKeyFound() const;
	void setKeyFound(bool keyFound);
	int getSockFd() const;
	void setSockFd(int sockFd);
};

#endif /* KEYINFO_H_ */
