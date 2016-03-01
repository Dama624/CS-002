CC = g++
CFLAGS = -Wall -ansi -pedantic -ggdb -std=c++11
LIBS = -lncurses -lpthread

.PHONY: clean

philosophers: philosophers.cpp
	$(CC) $(CFLAGS) philosophers.cpp -lncurses -o philosophers -lpthread

clean:
	rm -f *.o philosophers

