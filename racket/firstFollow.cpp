#include "firstFollow.hpp"
#include <iostream>
#include <algorithm>

namespace racket {

FirstFollowCalculator::FirstFollowCalculator() {
    initializeGrammar();
}

// Inicialização da Gramática

void FirstFollowCalculator::initializeGrammar() {
    // Define símbolo inicial
    startSymbol = "expr";
    
    // Define terminais
    terminals = {
        "IDENTIFIER", "NUMBER", "STRING", "BOOLEAN",
        "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACKET", "RIGHT_BRACKET",
        "QUOTE", "IF", "BEGIN", "PLAIN_APP",
        "DOT", "LAMBDA", "LET_VALUES", "LETREC_VALUES", "SET", "CASE_LAMBDA",
        "EOF", "EPSILON"
    };
    
    // Define não-terminais
    nonTerminals = {
        "expr", "literal", "list", "form",
        "quote", "if", "begin", "begin-rest", "app", "datum"
    };
    
    // Define produções
    // expr → literal | list
    Production exprProd("expr");
    exprProd.addAlternative({"literal"});
    exprProd.addAlternative({"list"});
    grammar.push_back(exprProd);
    
    // literal → IDENTIFIER | NUMBER | STRING | BOOLEAN
    Production literalProd("literal");
    literalProd.addAlternative({"IDENTIFIER"});
    literalProd.addAlternative({"NUMBER"});
    literalProd.addAlternative({"STRING"});
    literalProd.addAlternative({"BOOLEAN"});
    grammar.push_back(literalProd);
    
    // list → LEFT_PAREN form RIGHT_PAREN | LEFT_BRACKET form RIGHT_BRACKET
    Production listProd("list");
    listProd.addAlternative({"LEFT_PAREN", "form", "RIGHT_PAREN"});
    listProd.addAlternative({"LEFT_BRACKET", "form", "RIGHT_BRACKET"});
    grammar.push_back(listProd);
    
    // form → quote | if | begin | app
    Production formProd("form");
    formProd.addAlternative({"quote"});
    formProd.addAlternative({"if"});
    formProd.addAlternative({"begin"});
    formProd.addAlternative({"app"});
    grammar.push_back(formProd);
    
    // quote → QUOTE datum
    Production quoteProd("quote");
    quoteProd.addAlternative({"QUOTE", "datum"});
    grammar.push_back(quoteProd);
    
    // if → IF expr expr expr
    Production ifProd("if");
    ifProd.addAlternative({"IF", "expr", "expr", "expr"});
    grammar.push_back(ifProd);
    
    // begin → BEGIN expr begin-rest
    Production beginProd("begin");
    beginProd.addAlternative({"BEGIN", "expr", "begin-rest"});
    grammar.push_back(beginProd);

    // begin-rest → expr begin-rest | ε
    Production beginRestProd("begin-rest");
    beginRestProd.addAlternative({"expr", "begin-rest"});
    beginRestProd.addAlternative({"EPSILON"});
    grammar.push_back(beginRestProd);
    
    // app → PLAIN_APP expr expr*
    Production appProd("app");
    appProd.addAlternative({"PLAIN_APP", "expr"});
    grammar.push_back(appProd);
    
    // datum → expr
    Production datumProd("datum");
    datumProd.addAlternative({"expr"});
    grammar.push_back(datumProd);
}

// Metodos auxiliares

bool FirstFollowCalculator::isTerminal(const std::string& symbol) const {
    return terminals.find(symbol) != terminals.end();
}

bool FirstFollowCalculator::isNonTerminal(const std::string& symbol) const {
    return nonTerminals.find(symbol) != nonTerminals.end();
}

bool FirstFollowCalculator::canDeriveEpsilon(const std::string& symbol) const {
    if (symbol == "EPSILON") return true;
    if (isTerminal(symbol)) return false;
    
    auto it = firstSets.find(symbol);
    if (it == firstSets.end()) return false;
    
    return it->second.find("EPSILON") != it->second.end();
}

// Cálculo do FIRST

void FirstFollowCalculator::calculateFirst() {
    // Inicializa os conjuntos FIRST para terminais
    for (const auto& terminal : terminals) {
        if (terminal != "EPSILON") {
            firstSets[terminal].insert(terminal);
        }
    }
    
    // Inicializa os conjuntos FIRST para não-terminais como vazios
    for (const auto& nonTerminal : nonTerminals) {
        firstSets[nonTerminal] = std::set<std::string>();
    }
    
    // Itera até não haver mais mudanças (ponto fixo)
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Para cada produção A → α
        for (const auto& prod : grammar) {
            const std::string& A = prod.nonTerminal;
            
            for (const auto& alternative : prod.alternatives) {
                // Calcula FIRST(α)
                auto firstAlpha = firstOfSequence(alternative);
                
                // Adiciona FIRST(α) a FIRST(A)
                size_t oldSize = firstSets[A].size();
                firstSets[A].insert(firstAlpha.begin(), firstAlpha.end());
                
                if (firstSets[A].size() > oldSize) {
                    changed = true;
                }
            }
        }
    }
}

std::set<std::string> FirstFollowCalculator::firstOfSequence(
    const std::vector<std::string>& sequence) const {
    
    std::set<std::string> result;
    
    if (sequence.empty()) {
        result.insert("EPSILON");
        return result;
    }
    
    // Para cada símbolo na sequência
    for (size_t i = 0; i < sequence.size(); ++i) {
        const std::string& symbol = sequence[i];
        
        // Obtém FIRST(symbol)
        auto it = firstSets.find(symbol);
        if (it != firstSets.end()) {
            // Adiciona FIRST(symbol) - {ε} ao resultado
            for (const auto& terminal : it->second) {
                if (terminal != "EPSILON") {
                    result.insert(terminal);
                }
            }
            
            // Se o símbolo não pode derivar ε, pare
            if (!canDeriveEpsilon(symbol)) {
                return result;
            }
        } else {
            // Símbolo não encontrado, assume-se que é um terminal
            if (symbol != "EPSILON") {
                result.insert(symbol);
            }
            return result;
        }
    }
    
    // Todos os símbolos podem derivar ε
    result.insert("EPSILON");
    return result;
}

// Cálculo do FOLLOW

void FirstFollowCalculator::calculateFollow() {
    // Inicializa os conjuntos FOLLOW
    for (const auto& nonTerminal : nonTerminals) {
        followSets[nonTerminal] = std::set<std::string>();
    }
    
    // Regra 1: Adiciona EOF a FOLLOW(simbolo inicial)
    followSets[startSymbol].insert("EOF");
    
    // Itera até não haver mais mudanças (ponto fixo)
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Para cada produção A → α B β
        for (const auto& prod : grammar) {
            const std::string& A = prod.nonTerminal;
            
            for (const auto& alternative : prod.alternatives) {
                // Para cada símbolo na produção
                for (size_t i = 0; i < alternative.size(); ++i) {
                    const std::string& B = alternative[i];
                    
                    // Apenas processa não-terminais
                    if (!isNonTerminal(B)) continue;
                    
                    // Obtém β (resto da produção após B)
                    std::vector<std::string> beta(
                        alternative.begin() + i + 1,
                        alternative.end()
                    );
                    
                    // Regra 2: Adiciona FIRST(β) - {ε} a FOLLOW(B)
                    auto firstBeta = firstOfSequence(beta);
                    size_t oldSize = followSets[B].size();
                    
                    for (const auto& terminal : firstBeta) {
                        if (terminal != "EPSILON") {
                            followSets[B].insert(terminal);
                        }
                    }
                    
                    // Regra 3: Se β pode derivar ε, adiciona FOLLOW(A) a FOLLOW(B)
                    if (beta.empty() || firstBeta.find("EPSILON") != firstBeta.end()) {
                        followSets[B].insert(
                            followSets[A].begin(),
                            followSets[A].end()
                        );
                    }
                    
                    if (followSets[B].size() > oldSize) {
                        changed = true;
                    }
                }
            }
        }
    }
}

// Metodos de get

std::set<std::string> FirstFollowCalculator::getFirst(const std::string& symbol) const {
    auto it = firstSets.find(symbol);
    if (it != firstSets.end()) {
        return it->second;
    }
    return std::set<std::string>();
}

std::set<std::string> FirstFollowCalculator::getFollow(const std::string& nonTerminal) const {
    auto it = followSets.find(nonTerminal);
    if (it != followSets.end()) {
        return it->second;
    }
    return std::set<std::string>();
}

// Métodos de Print

void FirstFollowCalculator::printFirst() const {
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "CONJUNTO FISRT" << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    
    for (const auto& nonTerminal : nonTerminals) {
        auto it = firstSets.find(nonTerminal);
        if (it != firstSets.end()) {
            std::cout << "FIRST(" << nonTerminal << ") = { ";
            bool first = true;
            for (const auto& terminal : it->second) {
                if (!first) std::cout << ", ";
                std::cout << terminal;
                first = false;
            }
            std::cout << " }" << std::endl;
        }
    }
    std::cout << std::endl;
}

void FirstFollowCalculator::printFollow() const {
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "CONJUNTO FOLLOW" << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    
    for (const auto& nonTerminal : nonTerminals) {
        auto it = followSets.find(nonTerminal);
        if (it != followSets.end()) {
            std::cout << "FOLLOW(" << nonTerminal << ") = { ";
            bool first = true;
            for (const auto& terminal : it->second) {
                if (!first) std::cout << ", ";
                std::cout << terminal;
                first = false;
            }
            std::cout << " }" << std::endl;
        }
    }
    std::cout << std::endl;
}

// Verificação LL(1)

bool FirstFollowCalculator::isLL1() const {
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "VERIFICACAO LL(1)" << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    
    bool isLL1 = true;
    
    // Para cada não-terminal com múltiplas produções
    for (const auto& prod : grammar) {
        if (prod.alternatives.size() <= 1) continue;
        
        const std::string& A = prod.nonTerminal;
        
        // Verifica se os conjuntos FIRST das alternativas são disjuntos
        for (size_t i = 0; i < prod.alternatives.size(); ++i) {
            auto first_i = firstOfSequence(prod.alternatives[i]);
            
            for (size_t j = i + 1; j < prod.alternatives.size(); ++j) {
                auto first_j = firstOfSequence(prod.alternatives[j]);
                
                // Verifica interseção
                std::vector<std::string> intersection;
                std::set_intersection(
                    first_i.begin(), first_i.end(),
                    first_j.begin(), first_j.end(),
                    std::back_inserter(intersection)
                );
                
                if (!intersection.empty()) {
                    std::cout << "✗ " << A << ": CONJUNTO FIRST não disjunto" << std::endl;
                    std::cout << "  Alternativa " << i << " e " << j << " compartilham: ";
                    for (const auto& sym : intersection) {
                        std::cout << sym << " ";
                    }
                    std::cout << std::endl;
                    isLL1 = false;
                }
            }
        }
    }
    
    if (isLL1) {
        std::cout << "✓ Gramatica é LL(1)" << std::endl;
        std::cout << "  Todos os conjuntos FIRST são disjuntos" << std::endl;
    }
    
    std::cout << std::endl;
    return isLL1;
}

}