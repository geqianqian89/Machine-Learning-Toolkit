all:
	g++ -g -std=c++14 src/Utils.cpp src/Data.cpp src/main.cpp -o main.c
	#g++ -std=c++14 src/* -o main
clean:
	rm main*
