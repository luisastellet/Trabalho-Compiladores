#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <iomanip>
#include "syntaxTree.hpp"
#include <queue>
#include <unordered_map>
using namespace std;

struct DFA {
	int start;
	set<int> finals;
	vector<vector<int>> transitions;
};

struct NFA {
	int start;
	int end;
	vector<vector<set<int>>> transitions; 
};

// Calcula o delta do estado
set<int> delta(const NFA& nfa, const set<int>& states, int epsIdx) {
	set<int> closure = states;
	queue<int> queue;
	for (int state : states) queue.push(state);
	while (!queue.empty()) {
		int initialState = queue.front(); 
		queue.pop();
		for (int next : nfa.transitions[initialState][epsIdx]) {
			if (!closure.count(next)) {
				closure.insert(next);
				queue.push(next);
			}
		}
	}
	return closure;
}

DFA NFAtoDFA(const NFA& nfa, const vector<string>& alphabet) {
	int epsIdx = alphabet.size();
	map<set<int>, int> stateMap;
	vector<set<int>> dfaStates;
	vector<vector<int>> dfaTransictions;
	set<int> finals;
	int dfaStateCount = 0;

	// Estado inicial do DFA é o fecho-ε do estado inicial do NFA
	set<int> startSet = delta(nfa, {nfa.start}, epsIdx);
	// Mapear o estado inicial para o índice 0 do DFA
	stateMap[startSet] = dfaStateCount++;
	// Adicionar o estado inicial à lista de estados do DFA
	dfaStates.push_back(startSet);
	// Inicializar a matriz de transições do DFA com -1 (indicando ausência de transição)
	dfaTransictions.push_back(vector<int>(alphabet.size(), -1));

	// Processar os estados do DFA usando uma fila para explorar os estados alcançáveis
	queue<set<int>> queue;
	queue.push(startSet);

	while (!queue.empty()) {
		set<int> current = queue.front(); 
		queue.pop();
		int currentIdx = stateMap[current];
		for (size_t a = 0; a < alphabet.size(); ++a) {
			set<int> moveSet;
			for (int state : current) {
				for (int next : nfa.transitions[state][a]) {
					moveSet.insert(next);
				}
			}
			set<int> closure = delta(nfa, moveSet, epsIdx);
			if (closure.empty()) continue;
			if (!stateMap.count(closure)) {
				stateMap[closure] = dfaStateCount++;
				dfaStates.push_back(closure);
				dfaTransictions.push_back(vector<int>(alphabet.size(), -1));
				queue.push(closure);
			}
			dfaTransictions[currentIdx][a] = stateMap[closure];
		}
	}

	for (size_t i = 0; i < dfaStates.size(); ++i) {
		if (dfaStates[i].count(nfa.end)) finals.insert(i);
	}

	DFA dfa = {0, finals, dfaTransictions};
	return dfa;
}

void printDFA(const DFA& dfa, const vector<string>& alphabet) {
	cout << "Matriz de transições do Autômato Finito Determinístico:" << endl;
	cout << left << setw(10) << "Estado";
	for (const auto& sym : alphabet) cout << left << setw(12) << sym;
	cout << endl;
	for (size_t i = 0; i < dfa.transitions.size(); ++i) {
		cout << left << setw(10) << i;
		for (size_t j = 0; j < alphabet.size(); ++j) {
			string cell;
			if (dfa.transitions[i][j] == -1) cell = "-";
			else cell = to_string(dfa.transitions[i][j]);
			cout << left << setw(12) << cell;
		}
		cout << endl;
	}
	cout << "Estado inicial: " << dfa.start << "\nEstados finais: ";
	for (int f : dfa.finals) {
		cout << f << " ";
		// Verifica se há transições saindo de um estado final
		bool hasTransitions = false;
		for (size_t j = 0; j < alphabet.size(); ++j) {
			if (dfa.transitions[f][j] != -1) {
				hasTransitions = true;
				break;
			}
		}
		if (hasTransitions) {
			cout << "[AVISO: tem transições saindo!] ";
		}
	}
	cout << endl << endl;
}

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
	int n = nfaChild.transitions.size();
	int startState = n;  // novo início é logo após o child
	int f = n + 1;      // novo fim é logo após o novo início
	vector<vector<set<int>>> transitions = nfaChild.transitions;
	transitions.resize(n + 2, vector<set<int>>(alphabet.size() + 1));
	int eps = alphabet.size();
	cout << "[KLEENE*] Child: start=" << nfaChild.start << " end=" << nfaChild.end << " size=" << n 
	     << " | New: start=" << startState << " end=" << f << endl;
	// Liga novo início
	transitions[startState][eps].insert(nfaChild.start);
	// Liga novo início para novo fim (caso zero ocorrências)
	transitions[startState][eps].insert(f);
	// Liga fim do filho para início (loop)
	transitions[nfaChild.end][eps].insert(nfaChild.start);
	// Liga fim do filho para novo fim
	transitions[nfaChild.end][eps].insert(f);
	stateCount = f + 1;  // atualiza o contador global
	NFA nfa = {startState, f, transitions};
	return nfa;
}

NFA transictionPlus(NFA nfaChild, vector<string>& alphabet, int& stateCount) {
	int n = nfaChild.transitions.size();
	int startState = n;  // novo início é logo após o child
	int f = n + 1;      // novo fim é logo após o novo início
	vector<vector<set<int>>> transitions = nfaChild.transitions;
	transitions.resize(n + 2, vector<set<int>>(alphabet.size() + 1));
	int eps = alphabet.size();
	cout << "[PLUS+] Child: start=" << nfaChild.start << " end=" << nfaChild.end << " size=" << n 
	     << " | New: start=" << startState << " end=" << f << endl;
	// Liga novo início para início do filho
	transitions[startState][eps].insert(nfaChild.start);
	// Liga fim do filho para início (loop)
	transitions[nfaChild.end][eps].insert(nfaChild.start);
	// Liga fim do filho para novo fim
	transitions[nfaChild.end][eps].insert(f);
	stateCount = f + 1;  // atualiza o contador global
	NFA nfa = {startState, f, transitions};
	return nfa;
}

NFA transictionQuestion (NFA nfaChild, vector<string>& alphabet, int& stateCount) {
	int n = nfaChild.transitions.size();
	int startState = n;  // novo início é logo após o child
	int f = n + 1;      // novo fim é logo após o novo início
	vector<vector<set<int>>> transitions = nfaChild.transitions;
	transitions.resize(n + 2, vector<set<int>>(alphabet.size() + 1));
	int eps = alphabet.size();
	cout << "[QUESTION?] Child: start=" << nfaChild.start << " end=" << nfaChild.end << " size=" << n 
	     << " | New: start=" << startState << " end=" << f << endl;
	// Liga novo início para início do filho
	transitions[startState][eps].insert(nfaChild.start);
	// Liga novo início para novo fim 
	transitions[startState][eps].insert(f);
	// Liga fim do filho para novo fim
	transitions[nfaChild.end][eps].insert(f);
	stateCount = f + 1;  // atualiza o contador global
	NFA nfa = {startState, f, transitions};
	return nfa;
}

NFA transictionConcatenate (NFA left, NFA right, vector<string>& alphabet, int& stateCount){
	int offset = left.transitions.size();
	cout << "[CONCAT .] Left: start=" << left.start << " end=" << left.end << " size=" << left.transitions.size() 
	     << " | Right: start=" << right.start << " end=" << right.end << " size=" << right.transitions.size() << endl;
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
	stateCount = transitions.size();  // atualiza o contador global
	NFA nfa = {left.start, right.end + offset, transitions};
	cout << "  Result: start=" << nfa.start << " end=" << nfa.end << " total_states=" << transitions.size() << endl;
	return nfa;
}

NFA transictionPipe (NFA left, NFA right, vector<string>& alphabet, int& stateCount){
	cout << "[PIPE |] Left: start=" << left.start << " end=" << left.end << " size=" << left.transitions.size() 
	     << " | Right: start=" << right.start << " end=" << right.end << " size=" << right.transitions.size() << endl;
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
	stateCount = transitions.size();  // atualiza o contador global
	NFA nfa = {startState, f, transitions};
	cout << "  Result: start=" << nfa.start << " end=" << nfa.end << " total_states=" << transitions.size() << endl;
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
			return transictionConcatenate (left, right, alphabet, stateCount);

		} else if (node->getValue() == "|") { 
			return transictionPipe (left, right, alphabet, stateCount);

		} else {
			throw runtime_error("Operador binário desconhecido: " + node->getValue());
		}
	}
	int startState = stateCount++;
	int f = stateCount++;
	// Inicializa a matriz de transições do NFA com conjuntos vazios
	// O número de estados é f + 1 (de 0 a f) e o número de símbolos é alphabet.size() + 1 (incluindo ε)
	// Cada célula transitions[i][j] é um conjunto de estados alcançáveis a partir do estado i com o símbolo j
	vector<vector<set<int>>> transitions(f + 1, vector<set<int>>(alphabet.size() + 1));
	int idx = -1;
	for (size_t i = 0; i < alphabet.size(); ++i) {
		if (node->getValue() == alphabet[i]) idx = static_cast<int>(i);
	}
	if (idx == -1 && node->getValue() != "ε" && node->getValue() != "epsilon")
		throw runtime_error("Símbolo desconhecido: " + node->getValue());
	if (idx != -1) {
		transitions[startState][idx].insert(f);
		cout << "[SYMBOL] '" << node->getValue() << "' (idx=" << idx << "): " << startState << " -> " << f << endl;
	} else {
		transitions[startState][alphabet.size()].insert(f);
		cout << "[EPSILON] ε: " << startState << " -> " << f << endl;
	}
	NFA nfa = {startState, f, transitions};
	return nfa;
}

// imprimir a matriz do AFN que foi gerada
void printNFA(const NFA& nfa, const vector<string>& alphabet) {
	cout << "Matriz de transições do Autômato Finito Não Determinístico:" << endl;
	cout << left << setw(10) << "Estado";
	for (const auto& sym : alphabet) cout << left << setw(12) << sym;
	cout << left << setw(12) << "ε" << endl;
	for (size_t i = 0; i < nfa.transitions.size(); ++i) {
		cout << left << setw(10) << i;
		for (size_t j = 0; j < alphabet.size() + 1; ++j) {
			string cell = "";
			bool first = true;
			for (int st : nfa.transitions[i][j]) {
				if (!first) cell += ",";
				cell += to_string(st);
				first = false;
			}
			if (cell == "") cell = "-";
			cout << left << setw(12) << cell;
		}
		cout << endl;
	}
	cout << "Estado inicial: " << nfa.start << "\nEstado final: " << nfa.end << endl;
	cout << endl;
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
			printTree(tree);
			NFA nfa = buildNFA(tree, alphabet, stateCount);
			printNFA(nfa, alphabet);
			DFA dfa = NFAtoDFA(nfa, alphabet);
			printDFA(dfa, alphabet);
		} catch (const exception& e) {
			cout << "Erro ao construir AFN/DFA: " << e.what() << endl;
		}
		delete tree;
	}
	return 0;
}
