/**********************************
 * Name        : DESServer.cpp
 **********************************/

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "../DESAlgorithm.h"
#include "KeyNotifier.h"
#include "ServerSocket.h"
#include <bitset>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <cerrno>
#include <fstream>
#include <time.h>

using namespace std;

struct ThreadInput {
	int id;
	const string& input;
	const string& output;
	const string& inputToVerify;
	const string& outputToVerify;
	long double start;
	long double end;
	KeyNotifier& keyNotifier;
	int numberOfRounds;

	ThreadInput(int id, const string& input, const string& output,
			const string& inputToVerify, const string& outputToVerify,
			long double start, long double end, KeyNotifier& keyNotifier,
			int numberOfRounds) :
			id(id), input(input), output(output), inputToVerify(inputToVerify), outputToVerify(
					outputToVerify), start(start), end(end), keyNotifier(
					keyNotifier), numberOfRounds(numberOfRounds) {
	}
};

void *run(void *input) {

	ThreadInput* temp = (ThreadInput*) input;
	KeyNotifier& keyNotifier = temp->keyNotifier;
	DESAlgorithm* desObj = new DESAlgorithm(temp->input, temp->numberOfRounds);

	int myId = temp->id;
	cout << "\nThread id: " << temp->id << " DES input: " << temp->input
			<< " , output: " << temp->output;

	string key;
	string encryptedMessage;
	cout.precision(20);
	for (long double k = temp->start; k <= temp->end;) {
		if (keyNotifier.isKeyFound())
			break;
		key = bitset<64>(k).to_string();
		cout << "\nt_id: " << myId << " Trying key: " << k << endl;
		encryptedMessage = desObj->encrypt(key);
		if (encryptedMessage.compare(temp->output) == 0) {
			cout << "Key found: " << key << endl;
			ofstream myfile;
			  myfile.open (key.c_str());
			  myfile << "This key was found\n";
			  myfile.close();
//			keyFoundBySelf = true;
			DESAlgorithm verification(temp->inputToVerify,
					temp->numberOfRounds);
			string output = verification.encrypt(key);
			if (output.compare(temp->outputToVerify) == 0) {
				keyNotifier.setKeyFound(true);
				break;
			}
		}
		k = k + 1;
	}

	if (keyNotifier.isKeyFound())
		cout << "\nThread id: " << myId << " key found. Exiting from thread "
				<< endl << endl;

	pthread_exit(NULL);

	cout << "\n\t Exiting from thread: " << temp->id << endl;
	return NULL;
}

string readFromFile(string fileName) {
	string dataRead;
	ifstream messageFile(fileName.c_str());
	if (messageFile.is_open()) {
		getline(messageFile, dataRead);
		//cout << "message read from file "<< fileName <<": " << dataRead << endl;
		if (dataRead.empty()) {
			cout << "There is no content in the file " << fileName;
			exit(-1);
		}

		if (dataRead.length() != 64) {
			cout << "message length should be of 64 bits" << endl;
			exit(-1);
		}
	} else {
		cout << "unable to find " << fileName << " file\n";
		exit(-1);
	}
	return dataRead;
}

int main(int argc, char* argv[]) {

//	clock_t startedAt = clock();
	time_t startedAt = time(NULL);

	string messageToEncrypt = readFromFile("input1.txt");
	string encryptedMessage = readFromFile("output1.txt");

	string inputMessageToVerify = readFromFile("input2.txt");
	string outputMessageToVerify = readFromFile("output2.txt");

	if (argc < 2) {
		cout << "no port provided\n";
		cout << "Usage: " << argv[0] << " port [noOfRoundsForDES]\n";
		exit(-1);
	}

	int numberOfRounds = 16;
	if (argc == 3) {
		int numberOfRounds = atoi(argv[2]);
		if (numberOfRounds != 8 || numberOfRounds != 12
				|| numberOfRounds != 16) {
			cout << "Number of rounds should be 8, 12 or 16. " << endl;
			cout << "Usage: " << argv[0] << " port [noOfRoundsForDES]\n";
			exit(-1);
		}
	}

	KeyNotifier keyInfo;
	ServerSocket* serverSocket = new ServerSocket(atoi(argv[1]), keyInfo);
	serverSocket->acceptConnections(); // Waits till all 6 clients got connected.

	int retVal;

	int numOfThreads = 5;
	pthread_t threads[numOfThreads];

	long double start = 2635249153387078805;
//	long double start = 2635249153222078805;
	long double end;
	for (int i = 0; i < numOfThreads; i++) {
		end = start + 527049830677415760;
//		end = start + 527049830512415760;

		ThreadInput* input = new ThreadInput(i + 1, messageToEncrypt,
				encryptedMessage, inputMessageToVerify, outputMessageToVerify,
				start, end, keyInfo, numberOfRounds);
		retVal = pthread_create(&threads[i], NULL, run, (void *) input);
		if (retVal) {
			cout << "Error in creating thread, reason: " << retVal << endl;
			exit(-1);
		}
		start = end + 1;
	}

	for (int i = 0; i < numOfThreads; i++) {
		retVal = pthread_join(threads[i], NULL);
		if (retVal) {
			cout << "Error in joining thread, reason: " << retVal << endl;
			exit(-1);
		}
	}

	if (keyInfo.isKeyFound()) {
		cout << "\n Key found, so shutting down socket\n";
		serverSocket->shutDownConnection();
	}
	cout << "Waiting for socket thread to close\n";

	serverSocket->waitForConnections();

	time_t finishedAt = time(NULL);

	ofstream myfile;
	myfile.open ("timetaken");
	myfile  << "Time taken to break the key: " << difftime(finishedAt, startedAt) << endl;
	myfile.close();

	cout << "Time taken to break the key: " << difftime(finishedAt, startedAt)
			<< endl;
	return 0;
}
