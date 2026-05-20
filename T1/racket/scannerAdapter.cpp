#include "scannerAdapter.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

namespace racket {

ScannerAdapter::ScannerAdapter(const std::string& tokensFile)
    : currentIndex(0), filename(tokensFile) {
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
        throw std::runtime_error("Nao foi possivel abrir o arquivo de tokens: " + filename);
    }
    
    std::string line;
    int lineNum = 1;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int type;
        std::string value;
        
        if (!(iss >> type)) {
            std::cerr << "Aviso: formato de token invalido na linha " << lineNum << std::endl;
            continue;
        }
        
        std::getline(iss >> std::ws, value);
        
        Token token = createTokenFromType(type, value, lineNum, 1);
        tokens.push_back(token);
        
        lineNum++;
    }
    
    file.close();
}

Token ScannerAdapter::createTokenFromType(int type, const std::string& value, int line, int column) {
    TokenType tokenType;
    
    switch (type) {
        case 0: tokenType = TokenType::LEFT_PAREN; break;
        case 1: tokenType = TokenType::RIGHT_PAREN; break;
        case 2: tokenType = TokenType::LEFT_BRACKET; break;
        case 3: tokenType = TokenType::RIGHT_BRACKET; break;
        case 4: tokenType = TokenType::IF; break;
        case 5: tokenType = TokenType::BEGIN; break;
        case 6: tokenType = TokenType::QUOTE; break;
        case 7: tokenType = TokenType::NUMBER; break;
        case 8: tokenType = TokenType::STRING; break;
        case 9: tokenType = TokenType::IDENTIFIER; break;
        case 10: tokenType = TokenType::END_OF_FILE; break;
        default: tokenType = TokenType::IDENTIFIER; break;
    }
    
    Token token(tokenType, value, line, column);
    
    if (tokenType == TokenType::NUMBER) {
        try {
            token.numValue = std::stod(value);
        } catch (...) {
            token.numValue = 0.0;
        }
    }
    
    return token;
}

} 