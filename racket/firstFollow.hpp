#ifndef FIRST_FOLLOW_HPP
#define FIRST_FOLLOW_HPP

#include "racketParser.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

namespace racket {

/**
 * @brief Representa uma produção da gramática
 * 
 * Exemplo: expr → literal | list
 * nonTerminal = "expr"
 * productions = [["literal"], ["list"]]
 */
struct Production {
    std::string nonTerminal;
    std::vector<std::vector<std::string>> alternatives;
    
    Production(const std::string& nt) : nonTerminal(nt) {}
    
    void addAlternative(const std::vector<std::string>& alt) {
        alternatives.push_back(alt);
    }
};

/**
 * @brief Calcula conjuntos FIRST e FOLLOW para uma gramática
 * 
 * Implementa os algoritmos clássicos de compiladores:
 * - FIRST(X): conjunto de terminais que podem iniciar strings derivadas de X
 * - FOLLOW(X): conjunto de terminais que podem aparecer imediatamente após X
 */
class FirstFollowCalculator {
public:
    /**
     * @brief Construtor - inicializa com a gramática Racket
     */
    FirstFollowCalculator();
    
    /**
     * @brief Calcula todos os conjuntos FIRST
     */
    void calculateFirst();
    
    /**
     * @brief Calcula todos os conjuntos FOLLOW
     */
    void calculateFollow();
    
    /**
     * @brief Obtém FIRST de um símbolo
     */
    std::set<std::string> getFirst(const std::string& symbol) const;
    
    /**
     * @brief Obtém FOLLOW de um não-terminal
     */
    std::set<std::string> getFollow(const std::string& nonTerminal) const;
    
    /**
     * @brief Imprime todos os conjuntos FIRST
     */
    void printFirst() const;
    
    /**
     * @brief Imprime todos os conjuntos FOLLOW
     */
    void printFollow() const;
    
    /**
     * @brief Verifica se a gramática é LL(1)
     */
    bool isLL1() const;
    
private:
    // Gramática
    std::vector<Production> grammar;
    std::set<std::string> terminals;
    std::set<std::string> nonTerminals;
    std::string startSymbol;
    
    // Conjuntos FIRST e FOLLOW
    std::map<std::string, std::set<std::string>> firstSets;
    std::map<std::string, std::set<std::string>> followSets;
    
    /**
     * @brief Inicializa a gramática Racket
     */
    void initializeGrammar();
    
    /**
     * @brief Verifica se um símbolo é terminal
     */
    bool isTerminal(const std::string& symbol) const;
    
    /**
     * @brief Verifica se um símbolo é não-terminal
     */
    bool isNonTerminal(const std::string& symbol) const;
    
    /**
     * @brief Calcula FIRST de uma sequência de símbolos
     */
    std::set<std::string> firstOfSequence(const std::vector<std::string>& sequence) const;
    
    /**
     * @brief Verifica se um símbolo pode derivar epsilon
     */
    bool canDeriveEpsilon(const std::string& symbol) const;
};

} 

#endif 