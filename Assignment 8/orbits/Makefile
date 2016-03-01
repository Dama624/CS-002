CXX = g++
CXXFLAGS = -Wall -ansi -pedantic -ggdb
LDFLAGS = -lm
GLFLAGS = -lglut -lGLU -lGL
DOCSGEN = doxygen

SRC = src
OBJ = obj
BIN = bin
DOCS = docs

.PHONY: orbits testsuite docs clean

orbits: $(BIN)/orbits

testsuite: $(BIN)/testsuite

$(BIN)/orbits: $(OBJ)/EulerIntegrator.o $(OBJ)/Solver.o $(SRC)/main.cpp
	$(CXX) $^ $(LDFLAGS) $(GLFLAGS) -o $@

$(BIN)/testsuite: $(OBJ)/Solver.o $(OBJ)/EulerIntegrator.o $(SRC)/testsuite.cpp
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ)/Solver.o: $(SRC)/Solver.cpp $(SRC)/Solver.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/EulerIntegrator.o: $(SRC)/EulerIntegrator.cpp $(SRC)/EulerIntegrator.hpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

docs:
	doxygen

clean:
	rm -f $(OBJ)/*.o $(BIN)/orbits $(BIN)/testsuite
	rm -rf $(DOCS)/*
