CFLAGS= -std=c++11 -g 
all: main.cpp grid.o utils.o agent.o
	g++ $(CFLAGS) main.cpp grid.o utils.o agent.o -o projet
agent.o: agent.h agent.cpp
	g++ -c $(CFLAGS) agent.cpp -o agent.o
grid.o: grid.hpp grid.cpp
	g++ -c $(CFLAGS) grid.cpp -o grid.o
utils.o: utils.hpp utils.cpp
	g++ -c $(CFLAGS) utils.cpp -o utils.o

clean:
	rm -f agent.o grid.o utils.o projet
