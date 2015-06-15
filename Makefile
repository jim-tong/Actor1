main:main.o Actor.o
	g++ -o main main.o Actor.o 
main.o:main.cpp Actor.h Buffer.h
	g++ -c main.cpp -std=c++11 -pthread
Actor.o:Actor.cpp Buffer.h
	g++ -c Actor.cpp -std=c++11 -pthread
