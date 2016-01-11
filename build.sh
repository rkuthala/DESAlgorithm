rm -r output

mkdir output

g++ DESAlgorithm.cpp server/*.cpp -lpthread -std=c++11 -o output/server

g++ DESAlgorithm.cpp client/*.cpp -lpthread -std=c++11 -o output/client1

g++ DESAlgorithm.cpp client/*.cpp -lpthread -std=c++11 -DCLIENT_2 -o output/client2

g++ DESAlgorithm.cpp client/*.cpp -lpthread -std=c++11 -DCLIENT_3 -o output/client3

g++ DESAlgorithm.cpp client/*.cpp -lpthread -std=c++11 -DCLIENT_4 -o output/client4

g++ DESAlgorithm.cpp client/*.cpp -lpthread -std=c++11 -DCLIENT_5 -o output/client5

g++ DESAlgorithm.cpp client/*.cpp -lpthread -std=c++11 -DCLIENT_6 -o output/client6

g++ TestCases.cpp DESAlgorithm.cpp -std=c++11 -o output/runTestCases

cp input1.txt input2.txt output1.txt output2.txt output