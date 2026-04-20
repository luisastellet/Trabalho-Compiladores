#ifndef DFA_HPP
#define DFA_HPP

#include <vector>
#include <set>
#include <string>

using namespace std;

class NFA;  // Forward declaration

class DFA {
public:
	int start;
	set<int> finals;
	vector<vector<int>> transitions;

	void minimize(const vector<string>& alphabet);
	void printDFA(const vector<string>& alphabet) const;
	NFA toNFA(const vector<string>& alphabet) const;

	static DFA unionDFAs(const vector<DFA>& dfas, const vector<string>& alphabet, int& stateCount);
};

#endif
