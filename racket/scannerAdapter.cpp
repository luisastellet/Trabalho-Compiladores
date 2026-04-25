#include "scannerAdapter.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

namespace racket {

// ============================================================================
// ScannerAdapter Implementation
// ============================================================================

ScannerAdapter::ScannerAdapter(const std::string& tokensFile)
    : currentIndex(0) {
    // Read tokens from tokens.txt file
    loadTokensFromFile(tokensFile);
}

Token ScannerAdapter::nextToken() {
    if (currentIndex >= tokens.size()) {
        return Token(TokenType::END_OF_FILE, "", 1, 1);
    }
    return tokens[currentIndex++];
}

Token ScannerAdapter::peek() {
    if (currentIndex >= tokens.size()) {
        return Token(TokenType::END_OF_FILE, "", 1, 1);
    }
    return tokens[currentIndex];
}

bool ScannerAdapter::hasNext() const {
    return currentIndex < tokens.size();
}

std::vector<Token> ScannerAdapter::tokenizeAll() {
    return tokens;
}

void ScannerAdapter::loadTokensFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open tokens file: " + filename);
    }
    
    std::string line;
    int lineNum = 1;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Parse format: <type> <value>
        std::istringstream iss(line);
        int type;
        std::string value;
        
        if (!(iss >> type)) {
            std::cerr << "Warning: Invalid token format at line " << lineNum << std::endl;
            continue;
        }
        
        // Read rest of line as value (may contain spaces)
        std::getline(iss >> std::ws, value);
        
        // Convert type to TokenType and create token
        Token token = createTokenFromType(type, value, lineNum, 1);
        tokens.push_back(token);
        
        lineNum++;
    }
    
    file.close();
}

Token ScannerAdapter::createTokenFromType(int type, const std::string& value, int line, int column) {
    // Map scanner token types to parser token types
    // Based on tokens.txt format from scanner.c
    TokenType tokenType;
    
    switch (type) {
        case 0: tokenType = TokenType::IF; break;
        case 1: tokenType = TokenType::IDENTIFIER; break;  // else is identifier
        case 2: tokenType = TokenType::NUMBER; break;
        case 3: tokenType = TokenType::STRING; break;
        case 4: tokenType = TokenType::IDENTIFIER; break;
        case 5: tokenType = TokenType::QUOTE; break;
        case 6: tokenType = TokenType::BEGIN; break;
        case 7: tokenType = TokenType::LEFT_PAREN; break;
        case 8: tokenType = TokenType::RIGHT_PAREN; break;
        case 9: tokenType = TokenType::LEFT_BRACKET; break;
        case 10: tokenType = TokenType::RIGHT_BRACKET; break;
        case 11: tokenType = TokenType::DOT; break;
        case 12: tokenType = TokenType::PLAIN_APP; break;
        case 13: tokenType = TokenType::LAMBDA; break;
        case 14: tokenType = TokenType::LET_VALUES; break;
        case 15: tokenType = TokenType::LETREC_VALUES; break;
        case 16: tokenType = TokenType::SET; break;
        case 17: tokenType = TokenType::CASE_LAMBDA; break;
        case 18: tokenType = TokenType::BOOLEAN; break;  // #t or #f
        case 19: tokenType = TokenType::END_OF_FILE; break;
        default: tokenType = TokenType::IDENTIFIER; break;
    }
    
    Token token(tokenType, value, line, column);
    
    // Set numeric value for numbers
    if (tokenType == TokenType::NUMBER) {
        try {
            token.numValue = std::stod(value);
        } catch (...) {
            token.numValue = 0.0;
        }
    }
    
    return token;
}

} // namespace racket

// Made with Bob
