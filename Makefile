all: compile
	./a.out

compile:
	g++ -g -std=c++11 main.cpp

val: compile
	valgrind ./a.out
