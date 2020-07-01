CC = g++
DEBUG = -g
CFLAGS = $(DEBUG) -std=c++0x
PROGS = bloodsport
MAKEFLAGS = -k 

all: $(PROGS)

bloodsport: 
	$(CC) $(CFLAGS) Stack.cpp Queue.cpp Creature.cpp main.cpp -o bloodsport

clean:
	rm -f $(PROGS) *.o *~

