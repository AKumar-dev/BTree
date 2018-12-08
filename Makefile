all: compile
	./a.out command.txt

compile:
	g++ -g -Wall -std=c++11 main.cpp

val: compile
	valgrind ./a.out
