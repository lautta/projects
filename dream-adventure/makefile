CC = g++
DEBUG = -g
CFLAGS = $(DEBUG) -std=c++0x
PROGS = dreams
MAKEFLAGS = -k 

all: $(PROGS)

dreams: 
	$(CC) $(CFLAGS) Player.cpp Room.cpp main.cpp -o dreams

clean:
	rm -f $(PROGS) *.o *~

