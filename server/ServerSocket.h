/*
 * ServerSocket.h
 */

#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

#include <pthread.h>
#include "KeyNotifier.h"

struct SocketReaderInput {
	int sockFD;
	KeyNotifier& keyInfo;
	SocketReaderInput(int sockFD, KeyNotifier& keyInfo) :
			sockFD(sockFD), keyInfo(keyInfo) {
	}
};


class ServerSocket {
public:
	ServerSocket(int portNum, KeyNotifier& keyInfo);
	virtual ~ServerSocket();
	void acceptConnections();
	void waitForConnections();
	void shutDownConnection();
private:
	int serverSocket;
	KeyNotifier& keyInfo;
	pthread_t socketThreads[6];
	int acceptClientRequest();
	void bindAndListenForConnection(int sockfd, int portno);
	static void *readFromSocket(void* obj);
};

#endif /* SERVERSOCKET_H_ */
