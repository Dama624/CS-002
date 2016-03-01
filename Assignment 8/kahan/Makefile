CC = g++
LD = g++
CPPFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS = 

SRCS = kahan.cpp

OBJS = $(SRCS:.cpp=.o)

all: kahan

kahan: $(OBJS)
	g++ -o $@ $^ $(LDFLAGS)

%.o : %.cpp
	$(CC) -c $(CPPFLAGS) $< -o $@

clean :
	rm -f kahan *.o
