CXX = g++
TARGET = hidato
SRC = algo.cpp
#FLTK_FLAGS := $(shell fltk-config --cxxflags --ldflags)

all: $(TARGET)

$(TARGET): $(SRC)
#	$(CXX) $(SRC) -o $(TARGET) $(FLTK_FLAGS)
	$(CXX) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
