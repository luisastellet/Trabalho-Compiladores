#ifndef NFA_HPP
#define NFA_HPP

#include <vector>
#include <set>
#include <string>
#include "../syntax/syntaxTree.hpp"

using namespace std;

class DFA;  
class NFA {
    private:
        set<int> delta(const set<int>& states, int epsIdx) const;

        static NFA transictionKleeneStar(NFA nfaChild, vector<string>& alphabet, int& stateCount);
        static NFA transictionPlus(NFA nfaChild, vector<string>& alphabet, int& stateCount);
        static NFA transictionQuestion(NFA nfaChild, vector<string>& alphabet, int& stateCount);
        static NFA transictionConcatenate(NFA left, NFA right, vector<string>& alphabet, int& stateCount);
        static NFA transictionPipe(NFA left, NFA right, vector<string>& alphabet, int& stateCount);

    public:
        int start;
        int end;
        vector<vector<set<int>>> transitions;

        DFA toDFA(const vector<string>& alphabet) const;
        void printNFA(const vector<string>& alphabet) const;

        static NFA buildNFA(Node* node, vector<string>& alphabet, int& stateCount);
};

#endif
