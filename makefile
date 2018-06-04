all:
	g++ -std=c++14 includes/* src/* -o main
	#g++ -std=c++14 src/* -o main
debug:
	g++ -g -std=c++14 src/Utils.cpp src/Data.cpp includes/Point.hpp src/main.cpp -o main
clean:
	rm main*
