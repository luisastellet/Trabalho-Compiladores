#include "dfa.hpp"
#include "nfa.hpp"
#include <iostream>
#include <map>
#include <set>
#include <iomanip>
#include <queue>
#include <sstream>

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
		emptyDFA.alphabet = alphabet;
		return emptyDFA;
	}

	// Construir mapas de símbolos: globalIdx -> localIdx para cada DFA
	// Isso permite mapear de um símbolo global para o índice local em cada DFA
	vector<map<size_t, size_t>> symMapPerDfa(dfas.size());
	
	for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
		const auto& localAlphabet = dfas[dfaIdx].alphabet;
		
		// Para cada símbolo no alfabeto global
		for (size_t globalIdx = 0; globalIdx < alphabet.size(); ++globalIdx) {
			// Procurar este símbolo no alfabeto local do DFA
			for (size_t localIdx = 0; localIdx < localAlphabet.size(); ++localIdx) {
				if (alphabet[globalIdx] == localAlphabet[localIdx]) {
					symMapPerDfa[dfaIdx][globalIdx] = localIdx;
					break;
				}
			}
		}
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

	int maxStates = 10000;  // limite de segurança
	while (!toProcess.empty() && (int)reversedStateMap.size() < maxStates) {
		vector<int> currentState = toProcess.front();
		toProcess.pop();

		// Para cada símbolo no alfabeto GLOBAL, calcular o próximo estado
		for (size_t globalSymIdx = 0; globalSymIdx < alphabet.size(); ++globalSymIdx) {
			vector<int> nextState;
			bool validTransition = false;

			// Aplicar transição para cada DFA individualmente
			for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
				int stateInDfa = currentState[dfaIdx];
				int nextStateInDfa = -1;

				// Se o símbolo existe no alfabeto local do DFA
				if (symMapPerDfa[dfaIdx].find(globalSymIdx) != symMapPerDfa[dfaIdx].end()) {
					size_t localSymIdx = symMapPerDfa[dfaIdx][globalSymIdx];
					
					// Verificar se existe transição válida
					if (stateInDfa >= 0 && stateInDfa < (int)dfas[dfaIdx].transitions.size() &&
						localSymIdx < dfas[dfaIdx].transitions[stateInDfa].size()) {
						nextStateInDfa = dfas[dfaIdx].transitions[stateInDfa][localSymIdx];
						if (nextStateInDfa != -1) {
							validTransition = true;
						}
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
	unionDfa.alphabet = alphabet;
	unionDfa.transitions = vector<vector<int>>(numUnionStates, vector<int>(alphabet.size(), -1));

	// Preencher as transições do DFA unido
	for (int id = 0; id < numUnionStates; ++id) {
		const auto& currentState = reversedStateMap[id];

		for (size_t globalSymIdx = 0; globalSymIdx < alphabet.size(); ++globalSymIdx) {
			vector<int> nextState;
			bool validTransition = false;

			// Aplicar transição para cada DFA individualmente
			for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
				int stateInDfa = currentState[dfaIdx];
				int nextStateInDfa = -1;

				// Se o símbolo existe no alfabeto local do DFA
				if (symMapPerDfa[dfaIdx].find(globalSymIdx) != symMapPerDfa[dfaIdx].end()) {
					size_t localSymIdx = symMapPerDfa[dfaIdx][globalSymIdx];
					
					// Verificar se existe transição válida
					if (stateInDfa >= 0 && stateInDfa < (int)dfas[dfaIdx].transitions.size() &&
						localSymIdx < dfas[dfaIdx].transitions[stateInDfa].size()) {
						nextStateInDfa = dfas[dfaIdx].transitions[stateInDfa][localSymIdx];
						if (nextStateInDfa != -1) {
							validTransition = true;
						}
					}
				}

				nextState.push_back(nextStateInDfa);
			}

			// Se houver transição válida, adicionar a transição no DFA unido
			if (validTransition && stateMap.find(nextState) != stateMap.end()) {
				unionDfa.transitions[id][globalSymIdx] = stateMap[nextState];
			}
		}
	}

	// Determinar os estados finais: estados onde PELO MENOS UM dos DFAs está em estado final
	// IMPORTANTE: excluir o estado inicial (que é um vetor de estados iniciais)
	for (int id = 0; id < numUnionStates; ++id) {
		const auto& compoundState = reversedStateMap[id];
		
		// Pular o estado inicial da união
		if (id == unionDfa.start) continue;

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

string DFA::generateCScanner(const vector<string>& alphabet) const {
	stringstream ss;
	int numStates = this->transitions.size();

	// Header
	ss << "#include <stdio.h>\n";
	ss << "#include <string.h>\n\n";

	// Estrutura para armazenar o resultado do scanner
	ss << "struct Token {\n";
	ss << "\tint type;        // ID do token (-1 = erro)\n";
	ss << "\tchar value[256]; // Valor do token\n";
	ss << "};\n\n";

	// Função scanner principal com switch case aninhado
	ss << "struct Token scanToken(const char* input, int* pos) {\n";
	ss << "\tstruct Token token;\n";
	ss << "\ttoken.type = -1;\n";
	ss << "\tmemset(token.value, 0, sizeof(token.value));\n\n";

	ss << "\tint currentState = " << this->start << ";\n";
	ss << "\tint lastFinalState = -1;\n";
	ss << "\tint lastFinalPos = *pos;\n";
	ss << "\tint tokenStart = *pos;\n\n";

	ss << "\twhile (input[*pos] != '\\0') {\n";
	ss << "\t\tchar c = input[*pos];\n";
	ss << "\t\tint nextState = -1;\n\n";

	ss << "\t\tswitch (currentState) {\n";

	// Para cada estado, criar um case
	for (int state = 0; state < numStates; ++state) {
		ss << "\t\t\tcase " << state << ":\n";
		ss << "\t\t\t\tswitch (c) {\n";

		// Para cada símbolo, criar uma transição
		bool hasTransitions = false;
		for (size_t symIdx = 0; symIdx < alphabet.size(); ++symIdx) {
			if (this->transitions[state][symIdx] != -1) {
				const string& sym = alphabet[symIdx];
				if (sym.length() == 1) {
					char c = sym[0];
					ss << "\t\t\t\t\tcase '" << c << "':\n";
					ss << "\t\t\t\t\t\tnextState = " << this->transitions[state][symIdx] << ";\n";
					ss << "\t\t\t\t\t\tbreak;\n";
					hasTransitions = true;
				} else if (sym == "\\b") {
					ss << "\t\t\t\t\tcase '\\b':\n";
					ss << "\t\t\t\t\t\tnextState = " << this->transitions[state][symIdx] << ";\n";
					ss << "\t\t\t\t\t\tbreak;\n";
					hasTransitions = true;
				}
				// Nota: strings multi-caractere como 'input' não podem ser tratadas
				// com switch simples, seria necessário usar strcmp
			}
		}

		if (!hasTransitions) {
			ss << "\t\t\t\t\tdefault:\n";
			ss << "\t\t\t\t\t\tnextState = -1;\n";
		}

		ss << "\t\t\t\t}\n";
		ss << "\t\t\t\tbreak;\n\n";
	}

	ss << "\t\t\tdefault:\n";
	ss << "\t\t\t\tnextState = -1;\n";
	ss << "\t\t\t\tbreak;\n";
	ss << "\t\t}\n\n";

	ss << "\t\tif (nextState == -1) break;\n\n";

	ss << "\t\tcurrentState = nextState;\n";
	ss << "\t\ttoken.value[*pos - tokenStart] = c;\n";
	ss << "\t\t(*pos)++;\n\n";

	ss << "\t\t// Verificar se é estado final (longest match)\n";
	ss << "\t\tswitch (currentState) {\n";

	for (int state = 0; state < numStates; ++state) {
		if (this->finals.count(state)) {
			ss << "\t\t\tcase " << state << ":\n";
			if (this->stateToToken.count(state)) {
				ss << "\t\t\t\tlastFinalState = " << this->stateToToken.at(state) << ";\n";
			}
			ss << "\t\t\t\tlastFinalPos = *pos;\n";
			ss << "\t\t\t\tbreak;\n";
		}
	}

	ss << "\t\t\tdefault:\n";
	ss << "\t\t\t\tbreak;\n";
	ss << "\t\t}\n";
	ss << "\t}\n\n";

	ss << "\t// Retornar ao último estado final (longest match)\n";
	ss << "\tif (lastFinalState != -1) {\n";
	ss << "\t\ttoken.type = lastFinalState;\n";
	ss << "\t\t*pos = lastFinalPos;\n";
	ss << "\t\ttoken.value[lastFinalPos - tokenStart] = '\\0';\n";
	ss << "\t}\n\n";

	ss << "\treturn token;\n";
	ss << "}\n\n";

	// Função main integrada que lê de arquivo
	ss << "int main() {\n";
	ss << "\tFILE* file = fopen(\"tests/test_input.txt\", \"r\");\n";
	ss << "\tif (!file) {\n";
	ss << "\t\tperror(\"Erro ao abrir tests/test_input.txt\");\n";
	ss << "\t\treturn 1;\n";
	ss << "\t}\n\n";

	ss << "\tprintf(\"Scanner Léxico Gerado\\n\");\n";
	ss << "\tprintf(\"═════════════════════════════════════\\n\\n\");\n\n";

	ss << "\tprintf(\"Expressões Regulares:\\n\");\n";
	ss << "\tprintf(\"  Token 0: a b . c . *       (ab, abc, abcc, ...)\\n\");\n";
	ss << "\tprintf(\"  Token 1: a \\\\b c . .       (a, \\\\b, c)\\n\");\n";
	ss << "\tprintf(\"  Token 2: a b . input * .   (ab, input, input*)\\n\");\n";
	ss << "\tprintf(\"  Token 3: a b . c ? d . . + (ab(c|d) repetido)\\n\\n\");\n\n";

	ss << "\tprintf(\"Testes:\\n\");\n";
	ss << "\tprintf(\"─────────────────────────────────────\\n\");\n\n";

	ss << "\tchar line[512];\n\n";

	ss << "\twhile (fgets(line, sizeof(line), file)) {\n";
	ss << "\t\t// Remover quebra de linha\n";
	ss << "\t\tsize_t len = strlen(line);\n";
	ss << "\t\tif (len > 0 && line[len-1] == '\\n') {\n";
	ss << "\t\t\tline[len-1] = '\\0';\n";
	ss << "\t\t}\n";
	ss << "\t\t// Ignorar linhas vazias\n";
	ss << "\t\tif (strlen(line) == 0) continue;\n\n";

	ss << "\t\t// Remover aspas de contorno se existirem\n";
	ss << "\t\tchar* input = line;\n";
	ss << "\t\tchar inputDisplay[512];\n";
	ss << "\t\tstrcpy(inputDisplay, line);\n\n";

	ss << "\t\tif (line[0] == '\\\"' && line[strlen(line)-1] == '\\\"') {\n";
	ss << "\t\t\tinput = &line[1];\n";
	ss << "\t\t\tline[strlen(line)-1] = '\\0';\n";
	ss << "\t\t}\n\n";

	ss << "\t\tprintf(\"Entrada: %s → \", inputDisplay);\n\n";

	ss << "\t\tint pos = 0;\n";
	ss << "\t\tstruct Token token = scanToken(input, &pos);\n\n";

	ss << "\t\tif (token.type >= 0) {\n";
	ss << "\t\t\tprintf(\"Token %d ('%s')\\n\", token.type, token.value);\n";
	ss << "\t\t} else {\n";
	ss << "\t\t\tprintf(\"Não reconhecido\\n\");\n";
	ss << "\t\t}\n";
	ss << "\t}\n\n";

	ss << "\tfclose(file);\n";
	ss << "\tprintf(\"\\n\\n\");\n";
	ss << "\treturn 0;\n";
	ss << "}\n";

	return ss.str();
}
