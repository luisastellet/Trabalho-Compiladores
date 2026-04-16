#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include <vector>
#include <set>
#include <string>
#include "syntaxTree.hpp"
using namespace std;

struct NFA {
    int start;
    int end;
    std::vector<std::vector<std::set<int>>> transitions;
};

struct DFA {
    int start;
    std::set<int> finals;
    std::vector<std::vector<int>> transitions;
};


std::vector<std::string> getAlphabet(Node* node, std::set<std::string>& symbols);
NFA buildNFA(Node* node, std::vector<std::string>& alphabet, int& stateCount);
void printNFAMatrix(const NFA& nfa, const std::vector<std::string>& alphabet);

DFA NFAtoDFA(const NFA& nfa, const std::vector<std::string>& alphabet);
DFA minimizeDFA(const DFA& dfa, const std::vector<std::string>& alphabet);
NFA DFAtoNFA(const DFA& dfa, const std::vector<std::string>& alphabet);
DFA unionDFAs(const std::vector<DFA>& dfas, const std::vector<std::string>& alphabet, int& stateCount);
void printDFAMatrix(const DFA& dfa, const std::vector<std::string>& alphabet);

#endif