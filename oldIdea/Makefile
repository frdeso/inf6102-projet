CPPFLAGS=-g
all: Biome.o Runner.o  main.cpp
	g++ $(CPPFLAGS) main.cpp Biome.o Runner.o -o bestfit

Biome.o: Biome.h Biome.cpp
	g++ $(CPPFLAGS) -c Biome.cpp -o Biome.o

Runner.o: Runner.h Runner.cpp
	g++ $(CPPFLAGS) -c Runner.cpp -o Runner.o
clean:
	rm Runner.o Biome.o bestfit
