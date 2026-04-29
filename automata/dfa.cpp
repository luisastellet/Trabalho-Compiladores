#include "dfa.hpp"
#include "nfa.hpp"
#include <iostream>
#include <map>
#include <set>
#include <iomanip>
#include <queue>
#include <sstream>
#include <algorithm>

using namespace std;

void DFA::minimize(const vector<string>& alphabet) {
	int n = this->transitions.size();
	
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
		
		// Para cada estado, criar uma assinatura baseada em partição atual, transições
		for (int i = 0; i < n; ++i) {
			vector<int> sig;
			sig.push_back(partition[i]); 
			
			for (size_t j = 0; j < alphabet.size(); ++j) {
				if (this->transitions[i][j] == -1) {
					sig.push_back(-1);  
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
	set<pair<int,int>> addedTransitions;  
	
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
	
	// NFA tem um único estado final
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

	vector<map<size_t, size_t>> symMapPerDfa(dfas.size());
	
	for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
		const auto& localAlphabet = dfas[dfaIdx].alphabet;
		
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

	map<vector<int>, int> stateMap;
	vector<vector<int>> reversedStateMap;  
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

	int maxStates = 10000;  
	while (!toProcess.empty() && (int)reversedStateMap.size() < maxStates) {
		vector<int> currentState = toProcess.front();
		toProcess.pop();

		for (size_t globalSymIdx = 0; globalSymIdx < alphabet.size(); ++globalSymIdx) {
			vector<int> nextState;
			bool validTransition = false;

			// Aplicar transição para cada DFA individualmente
			for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
				int stateInDfa = currentState[dfaIdx];
				int nextStateInDfa = -1;

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

			for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
				int stateInDfa = currentState[dfaIdx];
				int nextStateInDfa = -1;

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

			if (validTransition && stateMap.find(nextState) != stateMap.end()) {
				unionDfa.transitions[id][globalSymIdx] = stateMap[nextState];
			}
		}
	}

	// Determinar os estados finais: estados onde PELO MENOS UM dos DFAs está em estado final
	for (int id = 0; id < numUnionStates; ++id) {
		const auto& compoundState = reversedStateMap[id];
		
		if (id == unionDfa.start) continue;

		for (size_t dfaIdx = 0; dfaIdx < dfas.size(); ++dfaIdx) {
			int stateInDfa = compoundState[dfaIdx];
			// Se o estado está em estado final em qualquer um dos DFAs
			if (dfas[dfaIdx].finals.count(stateInDfa)) {
				unionDfa.finals.insert(id);
				unionDfa.stateToToken[id] = dfaIdx;
				break;  
			}
		}
	}

	stateCount = numUnionStates;
	return unionDfa;
}

string DFA::generateCScanner(const vector<string>& alphabet, const vector<string>& tokenNames) const {
	stringstream ss;
	int numStates = this->transitions.size();

	// Header
	ss << "#include <stdio.h>\n";
	ss << "#include <string.h>\n";
	ss << "#include <ctype.h>\n\n";

	// Estrutura para armazenar o resultado do scanner
	ss << "struct Token {\n";
	ss << "\tint type;\n";
	ss << "\tchar value[256];\n";
	ss << "};\n\n";

	// Função helper: verificar se um caractere é separador
	ss << "int isSeparator(char c) {\n";
	ss << "\treturn c == '(' || c == ')' || c == '{' || c == '}' ||\n";
	ss << "\t       c == ';' || c == ',' || c == '=';\n";
	ss << "}\n\n";

	// Função helper para match de símbolos (incluindo multi-char)
	ss << "// Função helper para comparar símbolo no input\n";
	ss << "int matchSymbol(const char* input, int pos, const char* symbol) {\n";
	ss << "\tsize_t len = strlen(symbol);\n";
	ss << "\tif (strncmp(&input[pos], symbol, len) == 0) return len;\n";
	ss << "\treturn 0;\n";
	ss << "}\n\n";

	// Função para extrair próximo lexema (palavra completa, separada por separadores)
	ss << "// Extrai o próximo lexema (sequência completa de caracteres não-separadores)\n";
	ss << "// Se começar com aspas, extrai até a aspas de fechamento\n";
	ss << "int extractLexeme(const char* input, int pos, char* lexeme) {\n";
	ss << "\tint len = 0;\n";
	ss << "\t// Se começar com aspas, extrair string entre aspas\n";
	ss << "\tif (input[pos] == '\\\"') {\n";
	ss << "\t\tif (len < 255) lexeme[len++] = input[pos++];\n";
	ss << "\t\twhile (input[pos] != '\\0' && input[pos] != '\\\"') {\n";
	ss << "\t\t\tif (len < 255) lexeme[len++] = input[pos++];\n";
	ss << "\t\t}\n";
	ss << "\t\tif (input[pos] == '\\\"') {\n";
	ss << "\t\t\tif (len < 255) lexeme[len++] = input[pos++];\n";
	ss << "\t\t}\n";
	ss << "\t} else {\n";
	ss << "\t\t// Caso normal: extrair até separador ou espaço\n";
	ss << "\t\twhile (input[pos] != '\\0' && !isspace(input[pos]) && !isSeparator(input[pos])) {\n";
	ss << "\t\t\tif (len < 255) lexeme[len++] = input[pos++];\n";
	ss << "\t\t}\n";
	ss << "\t}\n";
	ss << "\tlexeme[len] = '\\0';\n";
	ss << "\treturn len;\n";
	ss << "}\n\n";

	// Função scanner que valida um lexema completo contra o DFA
	ss << "struct Token validateLexeme(const char* lexeme) {\n";
	ss << "\tstruct Token token;\n";
	ss << "\ttoken.type = -1;\n";
	ss << "\tmemset(token.value, 0, sizeof(token.value));\n";
	ss << "\tstrcpy(token.value, lexeme);\n\n";

	ss << "\tint currentState = " << this->start << ";\n";
	ss << "\tint lastFinalState = -1;\n";
	ss << "\tint lastFinalPos = 0;\n";
	ss << "\tint pos = 0;\n\n";

	ss << "\t// Tentar avançar através do DFA com o lexema completo\n";
	ss << "\twhile (lexeme[pos] != '\\0') {\n";
	ss << "\t\tint nextState = -1;\n";
	ss << "\t\tint symbolLen = 0;\n\n";

	ss << "\t\tswitch (currentState) {\n";

	// Para cada estado, criar um case
	for (int state = 0; state < numStates; ++state) {
		ss << "\t\t\tcase " << state << ": {\n";

		// Criar array de possíveis transições
		vector<pair<int, string>> transitions;
		for (size_t symIdx = 0; symIdx < alphabet.size(); ++symIdx) {
			if (this->transitions[state][symIdx] != -1) {
				transitions.push_back({this->transitions[state][symIdx], alphabet[symIdx]});
			}
		}

		// Ordenar por comprimento (mais longos primeiro para longest match)
		sort(transitions.begin(), transitions.end(), 
			[](const pair<int,string>& a, const pair<int,string>& b) {
				return a.second.length() > b.second.length();
			});

		// Gerar if-else chain para cada símbolo
		for (size_t i = 0; i < transitions.size(); ++i) {
			const string& sym = transitions[i].second;
			int nextSt = transitions[i].first;

			if (i == 0) ss << "\t\t\t\t";
			else ss << " else ";

			if (sym.length() == 1 && sym[0] != '\\' && sym[0] != '\'' && sym[0] != '\"') {
				// Caractere simples
				ss << "if (lexeme[pos] == '" << sym[0] << "') {\n";
				ss << "\t\t\t\t\tnextState = " << nextSt << ";\n";
				ss << "\t\t\t\t\tsymbolLen = 1;\n";
				ss << "\t\t\t\t}\n";
			} else {
				// String multi-caractere ou símbolo especial (escapar aspas se necessário)
				string escapedSym = "";
				for(char c : sym) {
					if(c == '\"' || c == '\\') escapedSym += '\\';
					escapedSym += c;
				}
				ss << "if ((symbolLen = matchSymbol(lexeme, pos, \"" << escapedSym << "\"))) {\n";
				ss << "\t\t\t\t\tnextState = " << nextSt << ";\n";
				ss << "\t\t\t\t}\n";
			}
		}

		ss << "\t\t\t\tbreak;\n";
		ss << "\t\t\t}\n\n";
	}

	ss << "\t\t\tdefault:\n";
	ss << "\t\t\t\tnextState = -1;\n";
	ss << "\t\t\t\tbreak;\n";
	ss << "\t\t}\n\n";

	ss << "\t\tif (nextState == -1) break;\n\n";

	ss << "\t\tcurrentState = nextState;\n";
	ss << "\t\tpos += symbolLen;\n\n";

	ss << "\t\t// Verificar se é estado final (longest match)\n";
	ss << "\t\tswitch (currentState) {\n";

	for (int state = 0; state < numStates; ++state) {
		if (this->finals.count(state)) {
			ss << "\t\t\tcase " << state << ":\n";
			if (this->stateToToken.count(state)) {
				ss << "\t\t\t\tlastFinalState = " << this->stateToToken.at(state) << ";\n";
			}
			ss << "\t\t\t\tlastFinalPos = pos;\n";
			ss << "\t\t\t\tbreak;\n";
		}
	}

	ss << "\t\t\tdefault:\n";
	ss << "\t\t\t\tbreak;\n";
	ss << "\t\t}\n";
	ss << "\t}\n\n";

	ss << "\t// Retornar ao último estado final (longest match)\n";
	ss << "\t// MAS APENAS SE CONSUMIU TODO O LEXEMA\n";
	ss << "\tif (lastFinalState != -1 && lastFinalPos == strlen(lexeme)) {\n";
	ss << "\t\ttoken.type = lastFinalState;\n";
	ss << "\t} else {\n";
	ss << "\t\ttoken.type = -1;  // Lexema não corresponde a nenhum token\n";
	ss << "\t}\n\n";

	ss << "\treturn token;\n";
	ss << "}\n\n";

	// Função main integrada que lê de arquivo e gera tokens.txt
	ss << "int main(int argc, char* argv[]) {\n";
	// Nomes dos tokens
	ss << "\tconst char* tokenNames[] = {\n";
	for (size_t i = 0; i < tokenNames.size(); ++i) {
		ss << "\t\t\"" << tokenNames[i] << "\"" << (i == tokenNames.size() - 1 ? "" : ",") << "\n";
	}
	ss << "\t};\n\n";

	ss << "\t// Verificar argumentos\n";
	ss << "\tconst char* inputFile = (argc > 1) ? argv[1] : \"tests/test_input.txt\";\n";
	ss << "\tconst char* outputFile = \"tokens.txt\";\n\n";

	ss << "\tFILE* file = fopen(inputFile, \"r\");\n";
	ss << "\tif (!file) {\n";
	ss << "\t\tfprintf(stderr, \"Erro ao abrir arquivo: %s\\n\", inputFile);\n";
	ss << "\t\treturn 1;\n";
	ss << "\t}\n\n";

	ss << "\tFILE* tokensFile = fopen(outputFile, \"w\");\n";
	ss << "\tif (!tokensFile) {\n";
	ss << "\t\tfprintf(stderr, \"Erro ao criar arquivo: %s\\n\", outputFile);\n";
	ss << "\t\tfclose(file);\n";
	ss << "\t\treturn 1;\n";
	ss << "\t}\n\n";

	ss << "\tprintf(\"Scanner Léxico Gerado\\n\");\n";
	ss << "\tprintf(\"═════════════════════════════════════\\n\\n\");\n\n";

	ss << "\tprintf(\"Tokens definidos:\\n\");\n";
	for (size_t i = 0; i < tokenNames.size(); ++i) {
		ss << "\tprintf(\"  Token %zu: %s\\n\", (size_t)" << i << ", tokenNames[" << i << "]);\n";
	}
	ss << "\n";

	ss << "\tprintf(\"Testes:\\n\");\n";
	ss << "\tprintf(\"─────────────────────────────────────\\n\");\n\n";

	ss << "\tchar line[512];\n\n";

	ss << "\twhile (fgets(line, sizeof(line), file)) {\n";
	ss << "\t\t// Remover quebra de linha\n";
	ss << "\t\tsize_t len = strlen(line);\n";
	ss << "\t\tif (len > 0 && line[len-1] == '\\n') {\n";
	ss << "\t\t\tline[len-1] = '\\0';\n";
	ss << "\t\t}\n";
		ss << "\t\tif (len > 0 && line[len-1] == '\\r') {\n";
	ss << "\t\t\tline[len-1] = '\\0';\n";
	ss << "\t\t}\n";

	ss << "\t\t// Ignorar linhas vazias\n";
	ss << "\t\tif (strlen(line) == 0) continue;\n\n";

	ss << "\t\tprintf(\"Entrada: %s\\n\", line);\n\n";

	ss << "\t\tint pos = 0;\n";
	ss << "\t\twhile (line[pos] != '\\0') {\n";
	ss << "\t\t\t// Ignorar espaços em branco\n";
	ss << "\t\t\twhile (line[pos] != '\\0' && isspace(line[pos])) {\n";
	ss << "\t\t\t\tpos++;\n";
	ss << "\t\t\t}\n";
	ss << "\t\t\tif (line[pos] == '\\0') break;\n\n";

	ss << "\t\t\t// Fase 1: Separadores são tratados imediatamente\n";
	ss << "\t\t\tif (isSeparator(line[pos])) {\n";
	ss << "\t\t\t\tchar sep[2] = {line[pos], '\\0'};\n";
	ss << "\t\t\t\tstruct Token token = validateLexeme(sep);\n";
	ss << "\t\t\t\tif (token.type >= 0) {\n";
	ss << "\t\t\t\t\t// Escrever no arquivo tokens.txt\n";
	ss << "\t\t\t\t\tfprintf(tokensFile, \"%d %s\\n\", token.type, token.value);\n";
	ss << "\t\t\t\t\t// Imprimir na tela com tratamento de caracteres não-imprimíveis\n";
	ss << "\t\t\t\t\tprintf(\"  -> %s ('\", tokenNames[token.type]);\n";
	ss << "\t\t\t\t\tif (line[pos] >= 32 && line[pos] < 127) {\n";
	ss << "\t\t\t\t\t\tprintf(\"%c\", line[pos]);\n";
	ss << "\t\t\t\t\t} else {\n";
	ss << "\t\t\t\t\t\tprintf(\"[%d]\", (int)(unsigned char)line[pos]);\n";
	ss << "\t\t\t\t\t}\n";
	ss << "\t\t\t\t\tprintf(\"')\\n\");\n";
	ss << "\t\t\t\t} else {\n";
	ss << "\t\t\t\t\tprintf(\"  -> Não reconhecido: '%c'\\n\", line[pos]);\n";
	ss << "\t\t\t\t}\n";
	ss << "\t\t\t\tpos++;\n";
	ss << "\t\t\t} else {\n";
	ss << "\t\t\t\t// Fase 2: Extrair lexema completo (sequência de caracteres não-separadores)\n";
	ss << "\t\t\t\tchar lexeme[256];\n";
	ss << "\t\t\t\tint lexemeLen = extractLexeme(line, pos, lexeme);\n";
	ss << "\t\t\t\tif (lexemeLen > 0) {\n";
	ss << "\t\t\t\t\t// Validar o lexema completo contra todos os tokens\n";
	ss << "\t\t\t\t\tstruct Token token = validateLexeme(lexeme);\n";
	ss << "\t\t\t\t\tif (token.type >= 0) {\n";
	ss << "\t\t\t\t\t\t// Escrever no arquivo tokens.txt\n";
	ss << "\t\t\t\t\t\tfprintf(tokensFile, \"%d %s\\n\", token.type, token.value);\n";
	ss << "\t\t\t\t\t\t// Imprimir na tela\n";
	ss << "\t\t\t\t\t\tprintf(\"  -> %s ('%s')\\n\", tokenNames[token.type], token.value);\n";
	ss << "\t\t\t\t\t} else {\n";
	ss << "\t\t\t\t\t\tprintf(\"  -> Erro: '%s' não corresponde a nenhum token\\n\", lexeme);\n";
	ss << "\t\t\t\t\t}\n";
	ss << "\t\t\t\t\tpos += lexemeLen;\n";
	ss << "\t\t\t\t} else {\n";
	ss << "\t\t\t\t\tpos++; // Avança se não conseguiu extrair\n";
	ss << "\t\t\t\t}\n";
	ss << "\t\t\t}\n";
	ss << "\t\t}\n";
	ss << "\t\tprintf(\"\\n\");\n";
	ss << "\t}\n\n";

	ss << "\tfclose(file);\n";
	ss << "\tfclose(tokensFile);\n";
	ss << "\tprintf(\"\\n\");\n";
	ss << "\tprintf(\"Tokens salvos em: %s\\n\", outputFile);\n";
	ss << "\treturn 0;\n";
	ss << "}\n";

	return ss.str();
}
