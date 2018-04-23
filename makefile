all:
	$(CXX) -std=c++14 src/* -I . -Os -Wall -Wextra -o Example
clean:
	rm Example*
