#include "firstFollow.hpp"
#include <iostream>
#include <algorithm>

namespace racket {

// ============================================================================
// Constructor
// ============================================================================

FirstFollowCalculator::FirstFollowCalculator() {
    initializeGrammar();
}

// ============================================================================
// Grammar Initialization
// ============================================================================

void FirstFollowCalculator::initializeGrammar() {
    // Define start symbol
    startSymbol = "expr";
    
    // Define terminals
    terminals = {
        "IDENTIFIER", "NUMBER", "STRING", "BOOLEAN",
        "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACKET", "RIGHT_BRACKET",
        "QUOTE", "IF", "BEGIN", "PLAIN_APP",
        "DOT", "LAMBDA", "LET_VALUES", "LETREC_VALUES", "SET", "CASE_LAMBDA",
        "EOF", "EPSILON"
    };
    
    // Define non-terminals
    nonTerminals = {
        "expr", "literal", "list", "form",
        "quote", "if", "begin", "app", "datum"
    };
    
    // Define productions
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
    
    // begin → BEGIN expr expr*
    Production beginProd("begin");
    beginProd.addAlternative({"BEGIN", "expr"});
    grammar.push_back(beginProd);
    
    // app → PLAIN_APP expr expr*
    Production appProd("app");
    appProd.addAlternative({"PLAIN_APP", "expr"});
    grammar.push_back(appProd);
    
    // datum → expr
    Production datumProd("datum");
    datumProd.addAlternative({"expr"});
    grammar.push_back(datumProd);
}

// ============================================================================
// Helper Methods
// ============================================================================

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

// ============================================================================
// FIRST Calculation
// ============================================================================

void FirstFollowCalculator::calculateFirst() {
    // Initialize FIRST sets for terminals
    for (const auto& terminal : terminals) {
        if (terminal != "EPSILON") {
            firstSets[terminal].insert(terminal);
        }
    }
    
    // Initialize FIRST sets for non-terminals as empty
    for (const auto& nonTerminal : nonTerminals) {
        firstSets[nonTerminal] = std::set<std::string>();
    }
    
    // Iterate until no changes (fixed point)
    bool changed = true;
    while (changed) {
        changed = false;
        
        // For each production A → α
        for (const auto& prod : grammar) {
            const std::string& A = prod.nonTerminal;
            
            for (const auto& alternative : prod.alternatives) {
                // Calculate FIRST(α)
                auto firstAlpha = firstOfSequence(alternative);
                
                // Add FIRST(α) to FIRST(A)
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
    
    // For each symbol in the sequence
    for (size_t i = 0; i < sequence.size(); ++i) {
        const std::string& symbol = sequence[i];
        
        // Get FIRST(symbol)
        auto it = firstSets.find(symbol);
        if (it != firstSets.end()) {
            // Add FIRST(symbol) - {ε} to result
            for (const auto& terminal : it->second) {
                if (terminal != "EPSILON") {
                    result.insert(terminal);
                }
            }
            
            // If symbol cannot derive ε, stop
            if (!canDeriveEpsilon(symbol)) {
                return result;
            }
        } else {
            // Symbol not found, assume it's a terminal
            if (symbol != "EPSILON") {
                result.insert(symbol);
            }
            return result;
        }
    }
    
    // All symbols can derive ε
    result.insert("EPSILON");
    return result;
}

// ============================================================================
// FOLLOW Calculation
// ============================================================================

void FirstFollowCalculator::calculateFollow() {
    // Initialize FOLLOW sets
    for (const auto& nonTerminal : nonTerminals) {
        followSets[nonTerminal] = std::set<std::string>();
    }
    
    // Rule 1: Add EOF to FOLLOW(start symbol)
    followSets[startSymbol].insert("EOF");
    
    // Iterate until no changes (fixed point)
    bool changed = true;
    while (changed) {
        changed = false;
        
        // For each production A → α B β
        for (const auto& prod : grammar) {
            const std::string& A = prod.nonTerminal;
            
            for (const auto& alternative : prod.alternatives) {
                // For each symbol in the production
                for (size_t i = 0; i < alternative.size(); ++i) {
                    const std::string& B = alternative[i];
                    
                    // Only process non-terminals
                    if (!isNonTerminal(B)) continue;
                    
                    // Get β (rest of the production after B)
                    std::vector<std::string> beta(
                        alternative.begin() + i + 1,
                        alternative.end()
                    );
                    
                    // Rule 2: Add FIRST(β) - {ε} to FOLLOW(B)
                    auto firstBeta = firstOfSequence(beta);
                    size_t oldSize = followSets[B].size();
                    
                    for (const auto& terminal : firstBeta) {
                        if (terminal != "EPSILON") {
                            followSets[B].insert(terminal);
                        }
                    }
                    
                    // Rule 3: If β can derive ε, add FOLLOW(A) to FOLLOW(B)
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

// ============================================================================
// Getters
// ============================================================================

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

// ============================================================================
// Print Methods
// ============================================================================

void FirstFollowCalculator::printFirst() const {
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "FIRST SETS (Calculated Algorithmically)" << std::endl;
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
    std::cout << "FOLLOW SETS (Calculated Algorithmically)" << std::endl;
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

// ============================================================================
// LL(1) Verification
// ============================================================================

bool FirstFollowCalculator::isLL1() const {
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "LL(1) VERIFICATION" << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
    
    bool isLL1 = true;
    
    // For each non-terminal with multiple productions
    for (const auto& prod : grammar) {
        if (prod.alternatives.size() <= 1) continue;
        
        const std::string& A = prod.nonTerminal;
        
        // Check if FIRST sets of alternatives are disjoint
        for (size_t i = 0; i < prod.alternatives.size(); ++i) {
            auto first_i = firstOfSequence(prod.alternatives[i]);
            
            for (size_t j = i + 1; j < prod.alternatives.size(); ++j) {
                auto first_j = firstOfSequence(prod.alternatives[j]);
                
                // Check intersection
                std::vector<std::string> intersection;
                std::set_intersection(
                    first_i.begin(), first_i.end(),
                    first_j.begin(), first_j.end(),
                    std::back_inserter(intersection)
                );
                
                if (!intersection.empty()) {
                    std::cout << "✗ " << A << ": FIRST sets not disjoint" << std::endl;
                    std::cout << "  Alternative " << i << " and " << j << " share: ";
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
        std::cout << "✓ Grammar is LL(1)" << std::endl;
        std::cout << "  All FIRST sets are disjoint" << std::endl;
    }
    
    std::cout << std::endl;
    return isLL1;
}

} // namespace racket

// Made with Bob