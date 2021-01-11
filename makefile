CC=g++
CFLAGS= -std=c++2a

default: G_VRP-FC

G_VRP-FC: node.o functions.o main.o CSP_Algorithm.o
	$(CC) node.o CSP_Algorithm.o main.o functions.o -o G_VRP-FC

CSP_Algorithm.o: CSP_Algorithm.cpp CSP_Algorithm.h functions.h
	$(CC) $(CFLAGS) -c CSP_Algorithm.cpp

functions.o: node.h functions.cpp functions.h
	$(CC) $(CFLAGS) -c functions.cpp
	
node.o: node.cpp node.h
	$(CC) $(CFLAGS) -c node.cpp

main.o: main.cpp node.h functions.h CSP_Algorithm.h
	$(CC) $(CFLAGS) -c main.cpp

clean:
	$(RM) G_VRP-FC *.o *~