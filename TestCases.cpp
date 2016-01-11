/*
 * TestCases.cpp
 */

#include <iostream>

#include "DESAlgorithm.h"

using namespace std;

int main(int argc, char* argv[])
{

	string plainText = "1000010111101000000100110101010000001111000010101011010000000101";
	string key1 = "0000000000000000000000000000000000000000000010000000000000000000";
	string key2 = "0000000000000000000000000000000000000000000100000000000000000000";
	string key3 = "0000000000000000000000000000000000000000001000000000000000000000";

	cout << "\n\nplain Text : " << plainText << endl;

	DESAlgorithm desObj(plainText, 16);

	cout << "Case 1: \n";
	cout << "key        : " << key1 << endl;
	cout << "cipher Text: " << desObj.encrypt(key1) << endl;
	cout << endl << endl;

	cout << "Case 2: \n";
	cout << "key        : " << key2 << endl;
	cout << "cipher Text: " << desObj.encrypt(key2) << endl;
	cout << endl << endl;

	cout << "Case 3: \n";
	cout << "key        : " << key3 << endl;
	cout << "cipher Text: " << desObj.encrypt(key3) << endl;
	cout << endl << endl;
	return 0;
}



