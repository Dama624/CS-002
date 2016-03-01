# This is a makefile!

SRCDIR = src
OBJDIR = obj
BINDIR = bin
DOCDIR = doc

CC = g++
LD = g++
DOCGEN = doxygen
CFLAGS = 
CPPFLAGS = -std=c++0x -I$(SRCDIR) -g 
ifeq "$(CFLAGS)" "-DNO_GFX"
LDFLAGS = -lm -lsqlite3
else
LDFLAGS = -lm -lGL -lGLU -lglut -lsqlite3 `sdl-config --libs`
endif
DOCGENFLAGS = 
COMMON_SRCS = common.cpp error.cpp star.cpp starmap.cpp
GRAPHVIEW_SRCS = graphs.cpp
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
GRAPHVIEW_OBJS = $(COMMON_OBJS) $(GRAPHVIEW_SRCS:.cpp=.o)
GRAPHVIEW_BINS = graphview
BINS = $(GRAPHVIEW_BINS)

all : $(BINS)

docs :
	$(DOCGEN) $(DOCGENFLAGS)

graphview : $(addprefix $(OBJDIR)/,$(GRAPHVIEW_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS) 

$(OBJDIR)/%.o : $(SRCDIR)/$(notdir %.cpp)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@ 

clean:
	rm -f $(addprefix $(BINDIR)/,$(BINS)) $(OBJDIR)/*.o

clean-docs:
	rm -r $(DOCDIR)/*
