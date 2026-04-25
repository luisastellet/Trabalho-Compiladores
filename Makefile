CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Targets
AUTOMATON_TARGET = automaton
RACKET_TARGET = racket_parser

# Source files
AUTOMATON_SRC = main/automaton.cpp syntax/syntaxTree.cpp automata/dfa.cpp automata/nfa.cpp
RACKET_SRC = main/racketMain.cpp racket/racketAST.cpp racket/racketParser.cpp racket/scannerAdapter.cpp racket/firstFollow.cpp

# Default target: build both
all: $(AUTOMATON_TARGET) $(RACKET_TARGET)

# Build automaton (Part 1)
$(AUTOMATON_TARGET): $(AUTOMATON_SRC)
	$(CXX) $(CXXFLAGS) -o main/$(AUTOMATON_TARGET) $(AUTOMATON_SRC)

# Build Racket parser (Part 2)
$(RACKET_TARGET): $(RACKET_SRC)
	$(CXX) $(CXXFLAGS) -o main/$(RACKET_TARGET) $(RACKET_SRC)

# Build only automaton
automaton: $(AUTOMATON_TARGET)

# Build only Racket parser
racket: $(RACKET_TARGET)

# Test Racket parser with tokens.txt
test-racket: $(RACKET_TARGET)
	@echo "=== Testing Racket Parser ==="
	./main/$(RACKET_TARGET) tokens.txt

# Run Racket parser in interactive mode
repl: $(RACKET_TARGET)
	./main/$(RACKET_TARGET) -i

# Clean all build artifacts
clean:
	rm -f main/$(AUTOMATON_TARGET) main/$(RACKET_TARGET)

.PHONY: all automaton racket test-racket repl clean
