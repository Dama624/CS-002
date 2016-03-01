CC = g++
LD = g++
CFLAGS = -Wall -ansi -pedantic -ggdb `sdl-config --cflags` -std=c++0x
SDLLIBS = `sdl-config --libs`
SRCDIR = src
OBJDIR = obj
BINDIR = bin

	
align: align.o
	$(LD) -o $@ $^

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	
clean:
	rm -f align.o align

