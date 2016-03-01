CXX = g++
CXXFLAGS = -Wall -ggdb --std=c++11
DOCSGEN = doxygen
LDFLAGS	= -lglut -lGLU -lGL -lm
SRC = src
OBJ = obj
BIN = bin
DOCS = docs

.PHONY: fourier visualizer docs

fourier: $(BIN)/fourier

visualizer: $(BIN)/visualizer

$(BIN)/fourier: $(OBJ)/ComplexNumber.o $(OBJ)/FourierTransform.o \
$(OBJ)/SpectralSynthesizer.o $(OBJ)/Vector3D.o
	$(CXX) $(SRC)/main.cpp $^ $(CXXFLAGS) $(LDFLAGS) -o $@

$(BIN)/visualizer: $(OBJ)/ComplexNumber.o $(OBJ)/FourierTransform.o \
$(OBJ)/visualizer.o
	$(CXX) $^ `sdl-config --libs` -lSDL_gfx -o $@

docs:
	$(DOCSGEN)

$(OBJ)/ComplexNumber.o: $(SRC)/ComplexNumber.cpp $(SRC)/ComplexNumber.hpp
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(OBJ)/FourierTransform.o: $(SRC)/FourierTransform.cpp $(SRC)/FourierTransform.hpp \
$(SRC)/ComplexNumber.hpp
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(OBJ)/visualizer.o: $(SRC)/visualizer.cpp $(SRC)/FourierTransform.hpp \
$(SRC)/ComplexNumber.hpp
	$(CXX) $< $(CXXFLAGS) `sdl-config --cflags` -c -o $@

$(OBJ)/SpectralSynthesizer.o: $(SRC)/SpectralSynthesizer.cpp \
$(SRC)/SpectralSynthesizer.hpp  $(SRC)/ComplexNumber.hpp \
$(SRC)/FourierTransform.hpp
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(OBJ)/Vector3D.o: $(SRC)/Vector3D.cpp $(SRC)/Vector3D.hpp
	$(CXX) $< -c $(CXXFLAGS) -o $@

clean:
	rm -f $(OBJ)/*.o $(BIN)/fourier $(BIN)/visualizer
	rm -rf $(DOCS)/*
