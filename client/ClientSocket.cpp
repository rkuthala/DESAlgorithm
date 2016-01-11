/*
 * ClientSocket.cpp
 */

#include "ClientSocket.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

ClientSocket::ClientSocket(KeyNotifier& keyInfo) :
		keyInfo(keyInfo) {

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		cout << "ERROR opening socket\n";
		exit(-1);
	}

}

ClientSocket::~ClientSocket() {
}

void ClientSocket::connectToServer(string hostName, int portNum) {
	sockaddr_in serverAddr;
	hostent *server;
	server = gethostbyname(hostName.c_str());
	if (server == NULL) {
		cout << "ERROR, no such host\n";
		exit(-1);
	}
	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serverAddr.sin_addr.s_addr,
			server->h_length);
	serverAddr.sin_port = htons(portNum);
	if (connect(clientSocket, (struct sockaddr *) &serverAddr,
			sizeof(serverAddr)) < 0) {
		cout << "ERROR connecting\n";
		exit(-1);
	}

	keyInfo.setSockFd(clientSocket);

	int retVal = pthread_create(&socketReader, NULL,
			&ClientSocket::readFromSocket, (void *) &keyInfo);
	if (retVal) {
		cout << "Error in creating thread, reason: " << retVal << endl;
		exit(-1);
	}

}

void *ClientSocket::readFromSocket(void* obj) {
	KeyNotifier* keyInfo = (KeyNotifier*) obj;
	int socketToReadFrom = keyInfo->getSockFd();
	cout << "Received sockfd: " << socketToReadFrom;
	char buffer[256];
	int n = read(socketToReadFrom, buffer, 255); // If we read something from socket, that indicates the key found
	if (n <= 0)
		cout << "ERROR reading from socket";
	else {
		cout << "data read from socket: " << buffer;
		keyInfo->setKeyFound(true);
	}

	return NULL;
}

void ClientSocket::waitForConnections() {
	cout << "Waiting for socket thread to close\n";
	pthread_join(socketReader, NULL);
}

void ClientSocket::shutDownConnection() {
	cout << "Key found, so shutting down connection\n";
	shutdown(clientSocket, SHUT_RDWR);
}

void ClientSocket::closeConnection() {
	cout << "socket thread to closed\n";
	close(clientSocket);
}

