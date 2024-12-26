all: Transmitter.o
	g++ -o a Transmitter.o

Transmitter.o: Transmitter.cpp
	g++ -Wall -g -c Transmitter.cpp -o Transmitter.o

clean:
	del Transmitter.o a.exe