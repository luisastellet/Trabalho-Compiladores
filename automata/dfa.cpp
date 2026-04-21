#include "dfa.hpp"
#include "nfa.hpp"
#include <iostream>
#include <map>
#include <set>
#include <iomanip>
#include <queue>

using namespace std;

void DFA::minimize(const vector<string>& alphabet) {
	int n = this->transitions.size();
	
	// Partição inicial: estados finais vs não-finais
	vector<int> partition(n);
	for (int i = 0; i < n; ++i) {
		partition[i] = this->finals.count(i) ? 1 : 0;
	}
	
	bool changed = true;
	while (changed) {
		changed = false;
		map<vector<int>, int> signature;
		vector<int> newPartition(n);
		int partNum = 0;
		
		// Para cada estado, criar uma assinatura baseada em: (partição atual, transições)
		for (int i = 0; i < n; ++i) {
			vector<int> sig;
			sig.push_back(partition[i]);  // partição atual do estado
			
			// Assinatura: a que partição cada transição leva
			for (size_t j = 0; j < alphabet.size(); ++j) {
				if (this->transitions[i][j] == -1) {
					sig.push_back(-1);  // sem transição
				} else {
					sig.push_back(partition[this->transitions[i][j]]);
				}
			}
			
			if (signature.find(sig) == signature.end()) {
				signature[sig] = partNum++;
			}
			newPartition[i] = signature[sig];
		}
		
		if (newPartition != partition) {
			changed = true;
			partition = newPartition;
		}
	}
	
	// Mapear estados antigos para novos
	map<int, int> stateMap;
	int newStateNum = 0;
	for (int i = 0; i < n; ++i) {
		if (stateMap.find(partition[i]) == stateMap.end()) {
			stateMap[partition[i]] = newStateNum++;
		}
	}
	
	// Construir novo DFA
	int newStart = stateMap[partition[this->start]];
	set<int> newFinals;
	for (int f : this->finals) {
		newFinals.insert(stateMap[partition[f]]);
	}
	
	// Nova matriz de transições
	vector<vector<int>> newTransitions(newStateNum, vector<int>(alphabet.size(), -1));
	set<pair<int,int>> addedTransitions;  // evitar duplicatas
	
	for (int i = 0; i < n; ++i) {
		int newI = stateMap[partition[i]];
		for (size_t j = 0; j < alphabet.size(); ++j) {
			if (this->transitions[i][j] != -1) {
				int newJ_target = stateMap[partition[this->transitions[i][j]]];
				if (addedTransitions.find({newI, j}) == addedTransitions.end()) {
					newTransitions[newI][j] = newJ_target;
					addedTransitions.insert({newI, j});
				}
			}
		}
	}
	
	this->start = newStart;
	this->finals = newFinals;
	this->transitions = newTransitions;
}

void DFA::printDFA(const vector<string>& alphabet) const {
	cout << "Matriz de transições do Autômato Finito Determinístico:" << endl;
	cout << left << setw(10) << "Estado";
	for (const auto& sym : alphabet) cout << left << setw(12) << sym;
	cout << endl;
	for (size_t i = 0; i < this->transitions.size(); ++i) {
		cout << left << setw(10) << i;
		for (size_t j = 0; j < alphabet.size(); ++j) {
			string cell;
			if (this->transitions[i][j] == -1) cell = "-";
			else cell = to_string(this->transitions[i][j]);
			cout << left << setw(12) << cell;
		}
		cout << endl;
	}
	cout << "Estado inicial: " << this->start << "\nEstados finais: ";
	for (int f : this->finals) {
		cout << f;
		if (!this->stateToToken.empty() && this->stateToToken.count(f)) {
			cout << " (Token " << this->stateToToken.at(f) << ")";
		}
		cout << " ";
	}
	cout << endl << endl;
}

NFA DFA::toNFA(const vector<string>& alphabet) const {
	int n = this->transitions.size();
	vector<vector<set<int>>> nfaTransitions(n, vector<set<int>>(alphabet.size() + 1));
	
	// Copiar transições do DFA para NFA
	// O DFA original pode ter um alfabeto menor, então precisamos apenas copiar o que existe
	for (int i = 0; i < n; ++i) {
		for (size_t j = 0; j < this->transitions[i].size(); ++j) {
			if (j < alphabet.size() && this->transitions[i][j] != -1) {
				nfaTransitions[i][j].insert(this->transitions[i][j]);
			}
		}
	}
	
	// NFA tem um único estado final (pegar o primeiro final - em caso de múltiplos, pegar qualquer um)
	int endState = *this->finals.begin();
	
	return {this->start, endState, nfaTransitions};
}

DFA DFA::unionDFAs(const vector<DFA>& dfas, const vector<string>& alphabet, int& stateCount) {
	if (dfas.empty()) {
		DFA emptyDFA;
		emptyDFA.start = 0;
		emptyDFA.transitions = vector<vector<int>>(1, vector<int>(alphabet.size(), -1));
		return emptyDFA;
	}

	// Mapeamento: estado composto (vetor de estados) -> ID numérico
	map<vector<int>, int> stateMap;
	vector<vector<int>> reversedStateMap;  // ID numérico -> vetor de estados
	queue<vector<int>> toProcess;

	// Estado inicial: concatenação de todos os estados iniciais
	vector<int> initialState;
	for (const auto& dfa : dfas) {
		initialState.push_back(dfa.start);
	}

	// BFS para explorar todos os estados alcançáveis
	stateMap[initialState] = 0;
	reversedStateMap.push_back(initialState);
	toProcess.push(initialState);

	while (!toProcess.empty()) {
		vector<int> currentState = toProcess.front();
		toProcess.pop();

		// Para cada símbolo no alfabeto, calcular o próximo estado
		for (size_t symIdx = 0; symIdx < alphabet.size(); ++symIdx) {
			vector<int> nextState;
			bool validTransition = false;

			// Aplicar transição para cada DFA individualmente
			for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
				int stateInDfa = currentState[dfaIdx];
				int nextStateInDfa = -1;

				// Verificar se existe transição válida
				if (stateInDfa >= 0 && stateInDfa < (int)dfas[dfaIdx].transitions.size() &&
					symIdx < dfas[dfaIdx].transitions[stateInDfa].size()) {
					nextStateInDfa = dfas[dfaIdx].transitions[stateInDfa][symIdx];
					if (nextStateInDfa != -1) {
						validTransition = true;
					}
				}

				nextState.push_back(nextStateInDfa);
			}

			// Se houver pelo menos uma transição válida, adicionar o novo estado
			if (validTransition && stateMap.find(nextState) == stateMap.end()) {
				stateMap[nextState] = reversedStateMap.size();
				reversedStateMap.push_back(nextState);
				toProcess.push(nextState);
			}
		}
	}

	// Construir o DFA unido
	int numUnionStates = reversedStateMap.size();
	DFA unionDfa;
	unionDfa.start = stateMap[initialState];
	unionDfa.transitions = vector<vector<int>>(numUnionStates, vector<int>(alphabet.size(), -1));

	// Preencher as transições do DFA unido
	for (int id = 0; id < numUnionStates; ++id) {
		const auto& currentState = reversedStateMap[id];

		for (size_t symIdx = 0; symIdx < alphabet.size(); ++symIdx) {
			vector<int> nextState;
			bool validTransition = false;

			// Aplicar transição para cada DFA individualmente
			for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
				int stateInDfa = currentState[dfaIdx];
				int nextStateInDfa = -1;

				// Verificar se existe transição válida
				if (stateInDfa >= 0 && stateInDfa < (int)dfas[dfaIdx].transitions.size() &&
					symIdx < dfas[dfaIdx].transitions[stateInDfa].size()) {
					nextStateInDfa = dfas[dfaIdx].transitions[stateInDfa][symIdx];
					if (nextStateInDfa != -1) {
						validTransition = true;
					}
				}

				nextState.push_back(nextStateInDfa);
			}

			// Se houver transição válida, adicionar a transição no DFA unido
			if (validTransition && stateMap.find(nextState) != stateMap.end()) {
				unionDfa.transitions[id][symIdx] = stateMap[nextState];
			}
		}
	}

	// Determinar os estados finais: estados onde PELO MENOS UM dos DFAs está em estado final
	for (int id = 0; id < numUnionStates; ++id) {
		const auto& compoundState = reversedStateMap[id];

		for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
			int stateInDfa = compoundState[dfaIdx];
			// Se o estado está em estado final em qualquer um dos DFAs
			if (dfas[dfaIdx].finals.count(stateInDfa)) {
				unionDfa.finals.insert(id);
				// Mapear o estado final para o token (ID = índice do DFA na lista)
				unionDfa.stateToToken[id] = dfaIdx;
				break;  // Já está marcado como final, não precisa verificar outros
			}
		}
	}

	stateCount = numUnionStates;
	return unionDfa;
}
