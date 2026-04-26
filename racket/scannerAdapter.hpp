#ifndef SCANNER_ADAPTER_HPP
#define SCANNER_ADAPTER_HPP

#include "racketParser.hpp"
#include <vector>
#include <string>

namespace racket {

/**
 * @brief Classe adaptadora para ler tokens do arquivo tokens.txt
 *
 * Este adaptador permite que o RacketParser use tokens gerados pelo
 * scanner.c (do automaton) por meio do arquivo tokens.txt.
 */
class ScannerAdapter {
public:
    /**
     * @brief Construct adapter from tokens file
     * @param tokensFile Path to tokens.txt file
     */
    explicit ScannerAdapter(const std::string& tokensFile);
    
    /**
     * @brief Get next token
     * @return Token compatible with RacketParser
     */
    Token nextToken();
    
    /**
     * @brief Peek at next token without consuming it
     * @return Next token
     */
    Token peek();
    
    /**
     * @brief Check if more tokens available
     * @return true if more tokens, false otherwise
     */
    bool hasNext() const;
    
    /**
     * @brief Get all tokens at once
     * @return Vector of all tokens
     */
    std::vector<Token> tokenizeAll();
    
    /**
     * @brief Get the tokens file name
     */
    const std::string& getFilename() const { return filename; }
    
private:
    std::vector<Token> tokens;
    size_t currentIndex;
    std::string filename;
    
    /**
     * @brief Load tokens from file
     * @param filename Path to tokens file
     */
    void loadTokensFromFile(const std::string& filename);
    
    /**
     * @brief Create Token from type and value
     * @param type Token type from scanner
     * @param value Token lexeme
     * @param line Line number
     * @param column Column number
     * @return Token for parser
     */
    Token createTokenFromType(int type, const std::string& value, int line, int column);
};

} 

#endif 