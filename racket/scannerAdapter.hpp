#ifndef SCANNER_ADAPTER_HPP
#define SCANNER_ADAPTER_HPP

#include "racketParser.hpp"
#include <vector>
#include <string>

namespace racket {

class ScannerAdapter {
public:

    explicit ScannerAdapter(const std::string& tokensFile);

    Token nextToken();
 
    Token peek();
 
    bool hasNext() const;
 
    std::vector<Token> tokenizeAll();

    const std::string& getFilename() const { return filename; }
    
private:
    std::vector<Token> tokens;
    size_t currentIndex;
    std::string filename;

    void loadTokensFromFile(const std::string& filename);

    Token createTokenFromType(int type, const std::string& value, int line, int column);
};
} 

#endif 