/*
 * ServerSocket.cpp
 */

#include "ServerSocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

ServerSocket::ServerSocket(int portNum, KeyNotifier& keyInfo) :
		keyInfo(keyInfo) {
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		cerr << "ERROR opening socket";
		exit(-1);
	}

	bindAndListenForConnection(serverSocket, portNum);
}

ServerSocket::~ServerSocket() {
}

void ServerSocket::acceptConnections() {
	int numberOfClients = 0;
	while (numberOfClients < 6) {
		cout << "waiting for request from client  " << numberOfClients << endl;
		int clientSocket = acceptClientRequest();

		cout << "connected sock fd: " << clientSocket << endl;

		keyInfo.setSockFd(clientSocket);

		SocketReaderInput* input = new SocketReaderInput(clientSocket, keyInfo);
		int retVal = pthread_create(&socketThreads[numberOfClients], NULL,
				&ServerSocket::readFromSocket, (void *) input);
		if (retVal) {
			cout << "Error in creating thread, reason: " << retVal << endl;
			exit(-1);
		}
		numberOfClients++;
	}
}

void *ServerSocket::readFromSocket(void* obj) {
	SocketReaderInput* input = (SocketReaderInput*) obj;
	KeyNotifier& keyInfo = input->keyInfo;
	int socketToReadFrom = input->sockFD;
	cout << "Received sockfd: " << socketToReadFrom;
	char buffer[256];
	int n = read(socketToReadFrom, buffer, 255);
	cout << "\n number of bytes read from client: " << n << "\n";
	if (n <= 0)
		cerr << "ERROR reading from socket";
	else {
		cout << "data read from socket: " << buffer;
		keyInfo.setKeyFound(true);
	}

	return NULL;
}

void ServerSocket::bindAndListenForConnection(int sockfd, int portno) {
	struct sockaddr_in serverAddr;

	bzero((char *) &serverAddr, sizeof(serverAddr));
//	portno = 8093;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(portno);
	cout << "binding socket" << endl;
	if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
		cerr << "ERROR on binding";
	cout << "socket bound" << endl;
	listen(sockfd, 6);
}

int ServerSocket::acceptClientRequest() {
	int acceptedSockFD;
	socklen_t cliLen;
	struct sockaddr_in clientAddr;
	cliLen = sizeof(clientAddr);
	acceptedSockFD = accept(serverSocket, (struct sockaddr *) &clientAddr,
			&cliLen);
	cout << "accepted client: newsockfd: " << acceptedSockFD << endl;
	return acceptedSockFD;
}

void ServerSocket::waitForConnections() {

	for (int i = 0; i < 6; i++) {
		int retVal = pthread_join(socketThreads[i], NULL);
		if (retVal) {
			cout << "Error in joining thread, reason: " << retVal << endl;
			exit(-1);
		}
	}

	close(serverSocket);
}

void ServerSocket::shutDownConnection()
{
	shutdown(serverSocket, SHUT_RD);
}
