# This is a makefile!

SRCDIR = src
OBJDIR = obj
BINDIR = bin
DOCDIR = doc

CC = g++
LD = g++
DOCGEN = doxygen
CFLAGS = 
CPPFLAGS = -std=c++0x -I$(SRCDIR) `pkg-config --cflags gtk+-2.0` -g
LDFLAGS = `pkg-config --libs gtk+-2.0`
DOCGENFLAGS = 
COMMON_SRCS = NetworkWrapper.cpp error.cpp
CLIENT0_SRCS = client0.cpp
SERVER0_SRCS = server0.cpp
CLIENT1_SRCS = client1.cpp
SERVER1_SRCS = server1.cpp
CLIENT2_SRCS = client2.cpp
SERVER2_SRCS = server2.cpp
TESTSUITE_SRCS = testsuite.cpp
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
CLIENT0_OBJS = $(COMMON_OBJS) $(CLIENT0_SRCS:.cpp=.o)
SERVER0_OBJS = $(COMMON_OBJS) $(SERVER0_SRCS:.cpp=.o)
CLIENT1_OBJS = $(COMMON_OBJS) $(CLIENT1_SRCS:.cpp=.o)
SERVER1_OBJS = $(COMMON_OBJS) $(SERVER1_SRCS:.cpp=.o)
CLIENT2_OBJS = $(COMMON_OBJS) $(CLIENT2_SRCS:.cpp=.o)
SERVER2_OBJS = $(COMMON_OBJS) $(SERVER2_SRCS:.cpp=.o)
TESTSUITE_OBJS = $(COMMON_OBJS) $(TESTSUITE_SRCS:.cpp=.o)
CLIENT_BINS = client0 client1 client2
SERVER_BINS = server0 server1 server2
TESTSUITE_BINS = testsuite
BINS = $(CLIENT_BINS) $(SERVER_BINS) $(TESTSUITE_BINS)

all : $(BINS)

docs :
	$(DOCGEN) $(DOCGENFLAGS)

client0 : $(addprefix $(OBJDIR)/,$(CLIENT0_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS) 

client1 : $(addprefix $(OBJDIR)/,$(CLIENT1_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS) 

client2 : $(addprefix $(OBJDIR)/,$(CLIENT2_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS) 
	
server0 : $(addprefix $(OBJDIR)/,$(SERVER0_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS) 
	
server1 : $(addprefix $(OBJDIR)/,$(SERVER1_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS) 

server2 : $(addprefix $(OBJDIR)/,$(SERVER2_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

testsuite: $(SRCDIR)/client2.hpp $(addprefix $(OBJDIR)/, $(TESTSUITE_OBJS))
	$(LD) -o $(BINDIR)/$@ $(addprefix $(OBJDIR)/, $(TESTSUITE_OBJS))  $(LDFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/$(notdir %.cpp)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@ 

clean:
	rm $(addprefix $(BINDIR)/,$(BINS)) $(OBJDIR)/*.o

clean-docs:
	rm -r $(DOCDIR)/*
