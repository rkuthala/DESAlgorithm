/*
 * ClientSocket.h
 */

#ifndef CLIENTSOCKET_H_
#define CLIENTSOCKET_H_
#include <string>
#include "KeyNotifier.h"

class ClientSocket {
public:
	ClientSocket(KeyNotifier& keyInfo);
	virtual ~ClientSocket();
	void connectToServer(std::string hostName, int portNum);
	void waitForConnections();
	void shutDownConnection();
	void closeConnection();
private:
	KeyNotifier& keyInfo;
	int clientSocket;
	pthread_t socketReader;
	static void *readFromSocket(void* obj);
};

#endif /* CLIENTSOCKET_H_ */
