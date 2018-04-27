all:
	g++ -std=c++14 src/Point.cpp src/main.cpp -o main
	#g++ -std=c++14 src/* -o main
clean:
	rm main*
