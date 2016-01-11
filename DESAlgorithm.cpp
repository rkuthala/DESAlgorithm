/*
 * DESAlgorithm.cpp
 */

#include "DESAlgorithm.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <bitset>
using namespace std;
//#define DEBUG
DESAlgorithm::DESAlgorithm(std::string input, int numberOfRounds) :
		input(input), numberOfRounds(numberOfRounds) {

	if (input.length() < 64) {
		cout << "Message length should be of 64 bits" << endl;
		exit(-1);
	}

	for (size_t var = 0; var < input.length(); var++) {
//		cout<< var+1 << ":" <<message[var] << "\t" << initial_message_permutation[var]<< ":" << message[initial_message_permutation[var]-1] << endl;
		messageAfterIP += input[initialMessagePermutation[var] - 1];
	}
#ifdef DEBUG
	printBytes(messageAfterIP);
#endif

}

DESAlgorithm::~DESAlgorithm() {
}

bool DESAlgorithm::verify() {
	return true;
}

void DESAlgorithm::printBytes(const std::string& msg, std::string debugMessage,
		int subGroupBits) {
	debugMessage = debugMessage.empty() ? "msg" : debugMessage;
	cout << endl;
	for (size_t var = 0; var < msg.length(); var = var + subGroupBits) {
		cout << msg.substr(var, subGroupBits) << " ";
	}
	cout << "\t<---\t" << debugMessage << endl;

}

std::string DESAlgorithm::rotateBits(std::string& input,
		int numberOfRotations) {
	string output;

//	if(numberOfRotations == 1) // Expecting function gets invoked for at least one rotation
	{
		char temp = input.at(0);
		output = input.erase(0, 1);
		output += temp;
	}

	if (numberOfRotations == 2) {
		char temp = output.at(0);
		output = output.erase(0, 1);
		output += temp;
	}

	return output;
}

void DESAlgorithm::generateKeys(const std::string& key) {
//	string key("ramsumkc");
//	string bitKey = "";
//	for (i = 0; i < key.size(); ++i) {
//		bitKey += bitset<8>(key[i]).to_string();
//	}
#ifdef DEBUG
	printBytes(key, "Trying key");
#endif

	size_t i;
	string permutedKey;
	for (i = 0; i < 56; i++) {
		permutedKey += key[initialKeyPermutaion[i] - 1];
	}
#ifdef DEBUG
	printBytes(permutedKey, "Permuted key", 7);
#endif

	string left = permutedKey.substr(0, 28);
	string right = permutedKey.substr(28);
#ifdef DEBUG
	printBytes(left, "Left sub key", 7);
	printBytes(right, "Right sub key", 7);
#endif

	string leftRotations[numberOfRounds];
	string rightRotations[numberOfRounds];
	string temporaryKeyAfterRotation[numberOfRounds];
	for (i = 0; i < numberOfRounds; i++) {
		leftRotations[i] = rotateBits(left, keyRotations[i]);
		rightRotations[i] = rotateBits(right, keyRotations[i]);
		left = leftRotations[i];
		right = rightRotations[i];
		temporaryKeyAfterRotation[i] = left + right;
	}

//	for (i = 0; i<16; i++)
//		printBytes(temporaryKeyAfterRotation[i], "Rotated key " +  i, 7);
//	return;
//	string finalKeys[16];
	for (i = 0; i < numberOfRounds; i++) {
		finalKeys[i] = "";
		for (int j = 0; j < 48; j++)
			finalKeys[i] += temporaryKeyAfterRotation[i][subKeyPermutation[j]
					- 1];
		//printBytes(finalKeys[i], "Key", 6);
	}
}

int DESAlgorithm::getDecimalVale(const std::string& input) {
	/*
	 * char a = '4';
	 * int ia = a - '0';
	 *
	 * input = 101
	 * decimalValue = 2^2 * 1 + 2^1 * 0 + 2^0 * 1
	 */
	int decimalValue = 0, base = 1;
	size_t totalBits = input.length();
	for (size_t i = 0; i < totalBits; i++) {
		decimalValue += (input[totalBits - i - 1] - '0') * base;
		base *= 2;
	}
	//cout << endl << endl << "The decimal value of " << input << " is:  " << decimalValue << endl;
	return decimalValue;
}

std::string DESAlgorithm::functionF(const std::string& right,
		const std::string& key) {
#ifdef DEBUG
	printBytes(right, "initial Right", 4);
#endif
	string response = "";
	std::string expandedRight;
	for (int i = 0; i < 48; i++) {
		expandedRight += right[messageExpansion[i] - 1];
	}
#ifdef DEBUG
	printBytes(key, "Key", 6);
	printBytes(expandedRight, "expanded right", 6);
#endif

	string rXorKey;
	char temp[10];
	for (int i = 0; i < 48; i++) {
		sprintf(temp, "%d",
				getDecimalVale(expandedRight.substr(i, 1))
						^ getDecimalVale(key.substr(i, 1)));
		rXorKey.append(temp);
	}
#ifdef DEBUG
	printBytes(rXorKey, "after XOR", 6);
#endif

	string sBoxesOutput;
	int currentBox = 1;
	for (int i = 0; i < 48; i += 6) {
		string sBoxInput = rXorKey.substr(i, 6);

		char rowValue[5];
		sprintf(rowValue, "%c%c", sBoxInput[0], sBoxInput[5]);
		int row = getDecimalVale(rowValue); //sBoxInput[0]+sBoxInput[5]);

		int column = getDecimalVale(sBoxInput.substr(1, 4));

		if (currentBox == 1)
			sBoxesOutput += sBox1[row][column];
		else if (currentBox == 2)
			sBoxesOutput += sBox2[row][column];
		else if (currentBox == 3)
			sBoxesOutput += sBox3[row][column];
		else if (currentBox == 4)
			sBoxesOutput += sBox4[row][column];
		else if (currentBox == 5)
			sBoxesOutput += sBox5[row][column];
		else if (currentBox == 6)
			sBoxesOutput += sBox6[row][column];
		else if (currentBox == 7)
			sBoxesOutput += sBox7[row][column];
		else if (currentBox == 8)
			sBoxesOutput += sBox8[row][column];

		currentBox++;
	}
#ifdef DEBUG
	printBytes(sBoxesOutput, "Output of sBoxe", 4);
#endif

	for (int i = 0; i < 32; i++) {
		response += sBoxesOutput[rightSubMessagePermutation[i] - 1];
	}
#ifdef DEBUG
	printBytes(response, "Final output of function", 4);
#endif
	return response;
}

/*void DESAlgorithm::intToBin(int value)
 {
 cout << "\n{";
 for(int i = 1; i <= 64; i++)
 {
 cout<<" \""<<bitset<4>(value)<<"\",";
 if(i != 0 && i%16 == 0)
 cout <<"},\n{";
 }
 }*/

std::string DESAlgorithm::getRightSubGroup(const std::string& left,
		const std::string& right, const std::string& key) {
	string response;
	string functionFResponse = functionF(right, key);

	char temp[10];
	for (int i = 0; i < 32; i++) {
		sprintf(temp, "%d",
				getDecimalVale(left.substr(i, 1))
						^ getDecimalVale(functionFResponse.substr(i, 1)));
		response.append(temp);
	}
#ifdef DEBUG
	printBytes(response, "Right subgroup", 4);
#endif
	return response;
}

string DESAlgorithm::encrypt(const std::string& key) {
#ifdef DEBUG
	printBytes(input);
#endif

	string left = messageAfterIP.substr(0, 32);
	string right = messageAfterIP.substr(32);
#ifdef DEBUG
	printBytes(left);
	printBytes(right);
#endif

	generateKeys(key);

	for (int round = 0; round < numberOfRounds; round++) {
		string leftSubGroup = right;
		string rightSubGroup = getRightSubGroup(left, right, finalKeys[round]); // left XOR function
		left = leftSubGroup;
		right = rightSubGroup;
	}
#ifdef DEBUG
	printBytes(left, "Final Left subgroup", 4);
	printBytes(right, "Final Right subgroup", 4);
#endif

	string tempOutput = right + left;
	string encryptedMessage;
	for (size_t i = 0; i < tempOutput.length(); i++) {
		encryptedMessage += tempOutput[finalMessagePermutation[i] - 1];
	}
#ifdef DEBUG
	printBytes(encryptedMessage, "Final Encrypted Message", 8);
#endif
	return encryptedMessage;
}
