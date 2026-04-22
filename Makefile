CXX = g++
TARGET = hidato_solver
SRC = hidato_solver.cpp
FLTK_FLAGS := $(shell fltk-config --cxxflags --ldflags)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(FLTK_FLAGS)

clean:
	rm -f $(TARGET)
