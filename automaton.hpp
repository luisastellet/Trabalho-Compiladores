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

std::vector<std::string> getAlphabet(Node* node, std::set<std::string>& symbols);
NFA buildNFA(Node* node, std::vector<std::string>& alphabet, int& stateCount);
void printNFAMatrix(const NFA& nfa, const std::vector<std::string>& alphabet);

#endif 