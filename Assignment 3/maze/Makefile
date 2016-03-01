# This is a makefile!

SRCDIR = src
OBJDIR = obj
BINDIR = bin
DOCDIR = docs

CC = g++
LD = g++
DOCGEN = doxygen
CFLAGS =
CPPFLAGS = -std=c++0x -I$(SRCDIR) -ggdb `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lSDL_gfx
DOCGENFLAGS =
COMMON_SRCS = CoordinateQueue.cpp CoordinateStack.cpp
MAZE_SRCS = $(COMMON_SRCS) BreadthFirstSolver.cpp DepthFirstSolver.cpp \
		MazeGrid.cpp MazeSolverApp.cpp RecursiveBacktracker.cpp
TESTSUITE_SRCS = $(COMMON_SRCS) testsuite.cpp
MAZE_OBJS = $(MAZE_SRCS:.cpp=.o)
TESTSUITE_OBJS = $(TESTSUITE_SRCS:.cpp=.o)
MAZE_BINS = maze
TESTSUITE_BINS = testsuite
BINS = $(MAZE_BINS) $(TESTSUITE_BINS)

.PHONY: maze testsuite clean clean-docs

all: $(BINS)

docs:
	$(DOCGEN) $(DOCGENFLAGS)

maze: $(addprefix $(SRCDIR)/,$(MAZE_SRCS))
	cd $(OBJDIR); $(CC) -c $(CFLAGS) $(CPPFLAGS) $(addprefix ../$(SRCDIR)/,$(MAZE_SRCS)); cd ..
	$(LD) -o $(BINDIR)/maze $(addprefix $(OBJDIR)/,$(MAZE_OBJS)) $(LDFLAGS)

testsuite: $(addprefix $(SRCDIR)/,$(TESTSUITE_SRCS))
	cd $(OBJDIR); $(CC) -c -DTESTSUITE $(CFLAGS) $(CPPFLAGS) $(addprefix ../$(SRCDIR)/,$(TESTSUITE_SRCS)); cd ..
	$(LD) -o $(BINDIR)/testsuite $(addprefix $(OBJDIR)/,$(TESTSUITE_OBJS)) $(LDFLAGS)

clean:
	rm -f $(addprefix $(BINDIR)/,$(BINS)) $(OBJDIR)/*.o

clean-docs:
	rm -rf $(DOCDIR)/*

