all:
	clang++ -std=c++14 src/* -o Example
clean:
	rm Example*
