CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
TARGET = automaton
SRC = automaton.cpp syntaxTree.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
