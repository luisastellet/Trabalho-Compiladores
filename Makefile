CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
TARGET = automaton
SRC = main/automaton.cpp syntax/syntaxTree.cpp automata/dfa.cpp automata/nfa.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o main/$(TARGET) $(SRC)

clean:
	rm -f main/$(TARGET)
