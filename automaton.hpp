#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include <vector>
#include <set>
#include <string>
#include <map>
#include <queue>
#include "syntaxTree.hpp"

class NFA; // Forward declaration
class DFA; // Forward declaration

class NFA {
private:
    // Compute epsilon closure
    std::set<int> delta(const std::set<int>& states, int epsIdx) const;

    // Transition builders (static private helpers)
    static NFA transictionKleeneStar(NFA nfaChild, std::vector<std::string>& alphabet, int& stateCount);
    static NFA transictionPlus(NFA nfaChild, std::vector<std::string>& alphabet, int& stateCount);
    static NFA transictionQuestion(NFA nfaChild, std::vector<std::string>& alphabet, int& stateCount);
    static NFA transictionConcatenate(NFA left, NFA right, std::vector<std::string>& alphabet, int& stateCount);
    static NFA transictionPipe(NFA left, NFA right, std::vector<std::string>& alphabet, int& stateCount);

public:
    int start;
    int end;
    std::vector<std::vector<std::set<int>>> transitions;

    // Convert NFA to DFA
    DFA toDFA(const std::vector<std::string>& alphabet) const;

    // Print NFA matrix
    void printNFA(const std::vector<std::string>& alphabet) const;

    // Build NFA from syntax tree
    static NFA buildNFA(Node* node, std::vector<std::string>& alphabet, int& stateCount);
};

class DFA {
public:
    int start;
    std::set<int> finals;
    std::vector<std::vector<int>> transitions;

    // Minimize DFA in place
    void minimize(const std::vector<std::string>& alphabet);

    // Print DFA matrix
    void printDFA(const std::vector<std::string>& alphabet) const;

    // Union of multiple DFAs
    static DFA unionDFAs(const std::vector<DFA>& dfas, const std::vector<std::string>& alphabet, int& stateCount);
};

// Utility function to extract alphabet from syntax tree
std::vector<std::string> getAlphabet(Node* node, std::set<std::string>& symbols);

#endif