CC = g++
LD = g++
CPPFLAGS = -std=c++0x -g -Wall -pedantic
LDFLAGS =

# Add any extra .cpp files to this line to
# have them compiled automatically.
SRCS = tictactoe.cpp game.cpp board.cpp

OBJS = $(SRCS:.cpp=.o)

all: tictactoe

tictactoe: $(OBJS)
	g++ -o $@ $^ $(LDFLAGS)

%.o : %.cpp
	$(CC) -c $(CPPFLAGS) $< -o $@

clean :
	rm -f tictactoe *.o
