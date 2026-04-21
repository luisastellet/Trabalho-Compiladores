#ifndef DFA_HPP
#define DFA_HPP

#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;

class NFA;  // Forward declaration

class DFA {
public:
	int start;
	set<int> finals;
	vector<vector<int>> transitions;
	map<int, int> stateToToken;  // Mapeia estado final -> ID do token (para scanner)

	void minimize(const vector<string>& alphabet);
	void printDFA(const vector<string>& alphabet) const;
	NFA toNFA(const vector<string>& alphabet) const;

	static DFA unionDFAs(const vector<DFA>& dfas, const vector<string>& alphabet, int& stateCount);
};

#endif
