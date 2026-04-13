#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "syntaxTree.hpp"
using namespace std;

struct NFA {
	int start;
	int end;
	vector<vector<set<int>>> transitions; 
};

// mapear símbolos para índices de coluna na matriz
vector<string> getAlphabet(Node* node, set<string>& symbols) {
	if (!node) return {};
	if (dynamic_cast<OperativeNodeUnary*>(node)) {
		OperativeNodeUnary* u = static_cast<OperativeNodeUnary*>(node);
		getAlphabet(u->getOperand(), symbols);
	} else if (dynamic_cast<OperativeNodeBinary*>(node)) {
		OperativeNodeBinary* b = static_cast<OperativeNodeBinary*>(node);
		getAlphabet(b->getLeftOperand(), symbols);
		getAlphabet(b->getRightOperand(), symbols);
	} else {
		if (!node->getValue().empty() && node->getValue() != "ε" && node->getValue() != "epsilon")
			symbols.insert(node->getValue());
	}
	return vector<string>(symbols.begin(), symbols.end());
}

NFA transictionKleeneStar(NFA nfaChild, vector<string>& alphabet, int& stateCount) {
	int startState = stateCount++;
	int f = stateCount++;
	int n = nfaChild.transitions.size();
	vector<vector<set<int>>> transitions(nfaChild.transitions);
	transitions.resize(n + 2, vector<set<int>>(alphabet.size() + 1));
	int eps = alphabet.size();
	// Liga novo início
	transitions[startState][eps].insert(nfaChild.start);
	// Liga novo início para novo fim (caso zero ocorrências)
	transitions[startState][eps].insert(f);
	// Liga fim do filho para início (loop)
	transitions[nfaChild.end][eps].insert(nfaChild.start);
	// Liga fim do filho para novo fim
	transitions[nfaChild.end][eps].insert(f);
	NFA nfa = {startState, f, transitions};
	return nfa;
}

NFA transictionPlus(NFA nfaChild, vector<string>& alphabet, int& stateCount) {
	int startState = stateCount++;
	int f = stateCount++;
	int n = nfaChild.transitions.size();
	vector<vector<set<int>>> transitions(nfaChild.transitions);
	transitions.resize(n + 2, vector<set<int>>(alphabet.size() + 1));
	int eps = alphabet.size();
	// Liga novo início para início do filho
	transitions[startState][eps].insert(nfaChild.start);
	// Liga fim do filho para início (loop)
	transitions[nfaChild.end][eps].insert(nfaChild.start);
	// Liga fim do filho para novo fim
	transitions[nfaChild.end][eps].insert(f);
	NFA nfa = {startState, f, transitions};
	return nfa;
}

NFA transictionQuestion (NFA nfaChild, vector<string>& alphabet, int& stateCount) {
	int startState = stateCount++;
	int f = stateCount++;
	int n = nfaChild.transitions.size();
	vector<vector<set<int>>> transitions(nfaChild.transitions);
	transitions.resize(n + 2, vector<set<int>>(alphabet.size() + 1));
	int eps = alphabet.size();
	// Liga novo início para
	transitions[startState][eps].insert(nfaChild.start);
	// Liga novo início para novo fim 
	transitions[startState][eps].insert(f);
	// Liga fim do filho para novo fim
	transitions[nfaChild.end][eps].insert(f);
	NFA nfa = {startState, f, transitions};
	return nfa;
}

NFA transictionConcatenate (NFA left, NFA right, vector<string>& alphabet){
	int offset = left.transitions.size();
	vector<vector<set<int>>> transitions(left.transitions);
	for (auto& row : right.transitions) {
		vector<set<int>> newRow(row);
		for (auto& startState : newRow) {
			set<int> newSet;
			for (int st : startState) newSet.insert(st + offset);
			startState = newSet;
		}
		transitions.push_back(newRow);
	}
	int eps = alphabet.size();
	// Liga fim do left para início do right
	transitions[left.end][eps].insert(right.start + offset);
	NFA nfa = {left.start, right.end + offset, transitions};
	return nfa;
}

NFA transictionPipe (NFA left, NFA right, vector<string>& alphabet, int& stateCount){
	int startState = stateCount++;
	int f = stateCount++;
	int offsetL = 1;
	int offsetR = left.transitions.size() + 1;
	int nL = left.transitions.size();
	int nR = right.transitions.size();
	int total = nL + nR + 2;
	vector<vector<set<int>>> transitions(total, vector<set<int>>(alphabet.size() + 1));
	int eps = alphabet.size();
	for (int i = 0; i < nL; ++i)
		for (size_t j = 0; j < alphabet.size() + 1; ++j)
			for (int st : left.transitions[i][j])
				transitions[i + offsetL][j].insert(st + offsetL);
	for (int i = 0; i < nR; ++i)
		for (size_t j = 0; j < alphabet.size() + 1; ++j)
			for (int st : right.transitions[i][j])
				transitions[i + offsetR][j].insert(st + offsetR);
	// Liga novo início
	transitions[startState][eps].insert(left.start + offsetL);
	// Liga novo início para início do right
	transitions[startState][eps].insert(right.start + offsetR);
	// Liga finais
	transitions[left.end + offsetL][eps].insert(f);
	// Liga fim do right para novo fim
	transitions[right.end + offsetR][eps].insert(f);
	NFA nfa = {startState, f, transitions};
	return nfa;
}

// construir o AFN a partir da syntax tree
NFA buildNFA(Node* node, vector<string>& alphabet, int& stateCount) {
	if (!node) throw runtime_error("Nó nulo na árvore sintática");
	// Operador unário
	if (dynamic_cast<OperativeNodeUnary*>(node)) {
		OperativeNodeUnary* u = static_cast<OperativeNodeUnary*>(node);
		NFA nfaChild = buildNFA(u->getOperand(), alphabet, stateCount);
		if (node->getValue() == "*") { 
			return transictionKleeneStar(nfaChild, alphabet, stateCount);
			
		} else if (node->getValue() == "+") { 
			return transictionPlus(nfaChild, alphabet, stateCount);
			
		} else if (node->getValue() == "?") { 
			return transictionQuestion (nfaChild, alphabet, stateCount);
			
		} else {
			throw runtime_error("Operador unário desconhecido: " + node->getValue());
		}
	}
	// Operador binário
	if (dynamic_cast<OperativeNodeBinary*>(node)) {
		OperativeNodeBinary* b = static_cast<OperativeNodeBinary*>(node);
		NFA left = buildNFA(b->getLeftOperand(), alphabet, stateCount);
		NFA right = buildNFA(b->getRightOperand(), alphabet, stateCount);
		if (node->getValue() == ".") { 
			return transictionConcatenate (left, right, alphabet);

		} else if (node->getValue() == "|") { 
			return transictionPipe (left, right, alphabet, stateCount);

		} else {
			throw runtime_error("Operador binário desconhecido: " + node->getValue());
		}
	}
	int startState = stateCount++;
	int f = stateCount++;
	vector<vector<set<int>>> transitions(f + 1, vector<set<int>>(alphabet.size() + 1));
	int idx = -1;
	for (size_t i = 0; i < alphabet.size(); ++i) {
		if (node->getValue() == alphabet[i]) idx = static_cast<int>(i);
	}
	if (idx == -1 && node->getValue() != "ε" && node->getValue() != "epsilon")
		throw runtime_error("Símbolo desconhecido: " + node->getValue());
	if (idx != -1)
		transitions[startState][idx].insert(f);
	else 
		transitions[startState][alphabet.size()].insert(f);
	NFA nfa = {startState, f, transitions};
	return nfa;
}

// imprimir a matriz do AFN que foi gerada
void printNFAMatrix(const NFA& nfa, const vector<string>& alphabet) {
	cout << "Matriz de transições (AFN):\n";
	cout << "Estados\\Símbolo\t";
	for (const auto& startState : alphabet) cout << startState << "\t";
	cout << "ε\n";
	for (size_t i = 0; i < nfa.transitions.size(); ++i) {
		cout << i << "\t\t";
		for (size_t j = 0; j < alphabet.size() + 1; ++j) {
			cout << "{";
			bool first = true;
			for (int st : nfa.transitions[i][j]) {
				if (!first) cout << ",";
				cout << st;
				first = false;
			}
			cout << "}\t";
		}
		cout << "\n";
	}
	cout << "Estado inicial: " << nfa.start << ", final: " << nfa.end << endl;
}


int main() {
	regularExpressionToken ret;
	vector<string> lines = readRegexFromFile("input.txt");
	if (lines.empty()) {
		cout << "Erro ao ler input.txt" << endl;
		return 1;
	}

	for (const auto& line : lines) {
		cout << "Expressão regular: " << line << endl;
		Node* tree = ret.createSyntaxTree(line);
		set<string> symbols;
		vector<string> alphabet = getAlphabet(tree, symbols);
		int stateCount = 0;
		try {
			NFA nfa = buildNFA(tree, alphabet, stateCount);
			printNFAMatrix(nfa, alphabet);
		} catch (const exception& e) {
			cout << "Erro ao construir AFN: " << e.what() << endl;
		}
		delete tree;
		cout << "-----------------------------" << endl;
	}
	return 0;
}
