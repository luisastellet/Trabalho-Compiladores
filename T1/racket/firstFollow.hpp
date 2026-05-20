#ifndef FIRST_FOLLOW_HPP
#define FIRST_FOLLOW_HPP

#include "racketParser.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

namespace racket {

struct Production {
    std::string nonTerminal;
    std::vector<std::vector<std::string>> alternatives;
    
    Production(const std::string& nt) : nonTerminal(nt) {}
    
    void addAlternative(const std::vector<std::string>& alt) {
        alternatives.push_back(alt);
    }
};

class FirstFollowCalculator {
public:
    FirstFollowCalculator();

    void calculateFirst();
    
    void calculateFollow();

    std::set<std::string> getFirst(const std::string& symbol) const;
    
    std::set<std::string> getFollow(const std::string& nonTerminal) const;
    
    void printFirst() const;

    void printFollow() const;
    
    bool isLL1() const;
    
private:
    std::vector<Production> grammar;
    std::set<std::string> terminals;
    std::set<std::string> nonTerminals;
    std::string startSymbol;
    
    std::map<std::string, std::set<std::string>> firstSets;
    std::map<std::string, std::set<std::string>> followSets;
    
    void initializeGrammar();
    
    bool isTerminal(const std::string& symbol) const;
    
    bool isNonTerminal(const std::string& symbol) const;

    std::set<std::string> firstOfSequence(const std::vector<std::string>& sequence) const;
    
    bool canDeriveEpsilon(const std::string& symbol) const;
};

} 

#endif 