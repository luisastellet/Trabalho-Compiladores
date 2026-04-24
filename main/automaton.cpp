#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include "../syntax/syntaxTree.hpp"
#include "../automata/dfa.hpp"
#include "../automata/nfa.hpp"

using namespace std;

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

int main() {
	regularExpressionToken ret;
	vector<string> lines = readRegexFromFile("./tests/regex.txt");
	if (lines.empty()) {
		cout << "Erro ao ler input.txt" << endl;
		return 1;
	}

	vector<DFA> minimizedDFAs;
	vector<string> tokenNames;
	set<string> globalSymbols;
	
	for (const auto& line : lines) {
		// Esperar formato: TOKEN_NAME: regex
		size_t colonPos = line.find(':');
		string tokenName = "TOKEN";  // Nome default caso não haja token especificado
		string regexLine = line;     // A parte da regex (pode ser a linha inteira se não houver token name)
		
		if (colonPos != string::npos) {
			tokenName = line.substr(0, colonPos);
			regexLine = line.substr(colonPos + 1);  // A parte da regex após os dois pontos
			// Tira o espaço extra no início da regex
			if (!regexLine.empty() && regexLine[0] == ' ') {
				regexLine = regexLine.substr(1);
			}
		}
		
		tokenNames.push_back(tokenName);
		
		cout << string(80, '=') << endl;
		cout << "Expressão regular: " << line << endl;
		cout << string(80, '=') << endl;
		Node* tree = ret.createSyntaxTree(regexLine);
		set<string> symbols;
		vector<string> alphabet = getAlphabet(tree, symbols);
		globalSymbols.insert(symbols.begin(), symbols.end());
		int stateCount = 0;
		try {
			printTree(tree);
			NFA nfa = NFA::buildNFA(tree, alphabet, stateCount);
			nfa.printNFA(alphabet);
			DFA dfa = nfa.toDFA(alphabet);
			cout << ">>> DFA ORIGINAL:" << endl;
			dfa.printDFA(alphabet);
			dfa.minimize(alphabet);
			cout << ">>> DFA MINIMIZADO:" << endl;
			dfa.printDFA(alphabet);
			minimizedDFAs.push_back(dfa);
		} catch (const exception& e) {
			cout << "Erro ao construir AFN/DFA: " << e.what() << endl;
		}
		delete tree;
	}
	
	// União de todos os DFAs minimizados
	if (minimizedDFAs.size() > 1) {
		cout << string(80, '=') << endl;
		cout << "DEBUG - Estados iniciais dos DFAs minimizados:" << endl;
		for (size_t i = 0; i < minimizedDFAs.size(); ++i) {
			cout << "DFA " << i << ": start=" << minimizedDFAs[i].start << ", finals={";
			for (int f : minimizedDFAs[i].finals) cout << f << " ";
			cout << "}" << endl;
		}
		cout << string(80, '=') << endl;
		cout << "AUTÔMATO FINAL - UNIÃO DE TODOS OS TOKENS:" << endl;
		cout << string(80, '=') << endl;
		try {
			vector<string> globalAlphabet(globalSymbols.begin(), globalSymbols.end());
			int stateCount = 0;
			DFA dfaUnion = DFA::unionDFAs(minimizedDFAs, globalAlphabet, stateCount);
			cout << ">>> DFA DA UNIÃO (sem minimizar - preserva contexto dos tokens):" << endl;
			dfaUnion.printDFA(globalAlphabet);  // Desabilitar matrix print - muito verboso
			
			// Gerar o scanner em C
			cout << string(80, '=') << endl;
			cout << "GERANDO SCANNER EM C..." << endl;
			cout << string(80, '=') << endl;
			string scannerCode = dfaUnion.generateCScanner(globalAlphabet, tokenNames);
			
			// Salvar em arquivo
			ofstream scannerFile("./scanner.c");
			if (scannerFile.is_open()) {
				scannerFile << scannerCode;
				scannerFile.close();
				cout << "Scanner gerado com sucesso: ./scanner.c" << endl;
			} else {
				cout << "Erro ao abrir arquivo scanner.c para escrita" << endl;
			}
		} catch (const exception& e) {
			cout << "Erro ao unir DFAs: " << e.what() << endl;
		}
	}
	
	cout << string(80, '=') << endl;
	return 0;
}
