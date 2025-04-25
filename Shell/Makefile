#Variables
CXX = g++
CC = gcc
CXXFLAGS = -Wall -Wextra -pedantic -std=c++20 -g -O3
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -O3
OBJECTS = tee.o head.o tail.o pwd.o wc.o 412shell.o
PROGRAMS = tee head tail pwd wc 412shell




# Targets and Dependencies


all: $(PROGRAMS)

tee: $(OBJECTS)
	$(CXX) $(CXXFLAGS) tee.o -o tee

tee.o: tee.cpp
	$(CXX) $(CXXFLAGS) -c tee.cpp -o tee.o 

tail: $(OBJECTS)
	$(CXX) $(CXXFLAGS) tail.o -o tail

tail.o: tail.cpp
	$(CXX) $(CXXFLAGS) -c tail.cpp -o tail.o

head: $(OBJECTS)
	$(CXX) $(CXXFLAGS) head.o -o head

head.o: head.cpp
	$(CXX) $(CXXFLAGS) -c head.cpp -o head.o

wc: $(OBJECTS)
	$(CXX) $(CXXFLAGS) wc.o -o wc

wc.o: wc.cpp
	$(CXX) $(CXXFLAGS) -c wc.cpp -o wc.o

pwd: $(OBJECTS)
	$(CXX) $(CXXFLAGS) pwd.o -o pwd

pwd.o: pwd.cpp
	$(CXX) $(CXXFLAGS) -c pwd.cpp -o pwd.o

412shell: $(OBJECTS)
	$(CC) $(CFLAGS) 412shell.o -o 412shell

412shell.o: 412shell.c
	$(CC) $(CFLAGS) -c 412shell.c -o 412shell.o


clean:
	rm -f $(OBJECTS) $(PROGRAMS)