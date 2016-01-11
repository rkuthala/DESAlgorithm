This program contatins the implementation of DES Algorithm and a brute force approach of finding plain text from a given cipher text.

1. Run build.sh to generate binaries for server, client1, client2, client3, client4, client5, client6 and runTestCases. These binaries will get copied into "output" folder.

2. run server (./server port_number [numberOfRounds]).

3. run clients (./client{1/2/3/4/5/6} hostname port_number [numberOfRounds])

4. The key space (2^64-1) is divided into 7 parts and given to each binary.

5. The input to each instance are passed through files.
	input1.txt  -> supplies initial plain text
	output1.txt -> supplies initial cipher text
	input2.txt  -> supplies plain text to verify after finding key
	output2.txt -> supplies cipher text to verify after finding key
	
6. Used 5 amazon web service(ubuntu) instances and 2 local(ubuntu) instances. Each instance runs 5 seperate threads.

7. The runTestCases executable displays the cipher text for given message with 3 different keys.