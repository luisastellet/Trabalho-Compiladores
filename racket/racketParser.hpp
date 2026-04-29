#ifndef RACKET_PARSER_HPP
#define RACKET_PARSER_HPP

#include "racketAST.hpp"
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <set>

namespace racket {

class ScannerAdapter;
class FirstFollowCalculator;

enum class TokenType {
    LEFT_PAREN,     
    RIGHT_PAREN,     
    LEFT_BRACKET,    
    RIGHT_BRACKET,   
    DOT,             
    
    IDENTIFIER,      
    NUMBER,          
    STRING,          
    BOOLEAN,         
    
    QUOTE,           
    IF,              
    BEGIN,           
    PLAIN_APP,       
    LAMBDA,          
    LET_VALUES,      
    LETREC_VALUES,   
    SET,             
    CASE_LAMBDA,     
    
    END_OF_FILE     
};


struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    double numValue;
    
    Token(TokenType type, const std::string& lexeme, int line, int column)
        : type(type), lexeme(lexeme), line(line), column(column), numValue(0.0) {}
    
    void print() const;
};

class ParseError : public std::runtime_error {
public:
    int line;
    int column;
    
    ParseError(const std::string& message, int line, int column)
        : std::runtime_error(message), line(line), column(column) {}
    
    std::string getFormattedMessage() const {
        std::ostringstream oss;
        oss << "Parse error at line " << line << ", column " << column 
            << ": " << what();
        return oss.str();
    }
};

class RacketParser {
public:

    explicit RacketParser(ScannerAdapter& scanner, bool useFirstFollow = true);
    
    ~RacketParser();
    
    std::unique_ptr<ExprNode> parse();

    std::vector<std::unique_ptr<ExprNode>> parseMultiple();
    

    const FirstFollowCalculator* getFirstFollowCalculator() const;

    bool hasErrors() const { return errorCount > 0; }
    
    int getErrorCount() const { return errorCount; }
    
private:
    ScannerAdapter& scanner;
    Token currentToken;
    FirstFollowCalculator* firstFollowCalc;
    bool useFirstFollowSets;
    int errorCount;
    std::string tokensFilename;
    
    void advance();
    
    bool check(TokenType type) const;

    bool checkAny(const std::vector<TokenType>& types) const;

    void consume(TokenType type, const std::string& message);

    bool isAtEnd() const;
 
    std::unique_ptr<ExprNode> parseExpr();
 
    std::unique_ptr<ExprNode> parseLiteral();
 
    std::unique_ptr<ExprNode> parseList();
 
    std::unique_ptr<ExprNode> parseQuote();
    
    std::unique_ptr<ExprNode> parseIf();
   
    std::unique_ptr<ExprNode> parseBegin();
    
    std::unique_ptr<ExprNode> parseApp();
    
    std::unique_ptr<ExprNode> parseDatum();

    [[noreturn]] void error(const std::string& message);

    void reportError(const std::string& message);

    bool panicModeRecover(const std::string& nonTerminal);

    void synchronize();
    
    bool inFirst(const std::string& nonTerminal) const;
    
    bool inFollow(const std::string& nonTerminal) const;
 
    std::string tokenTypeToString(TokenType type) const;
};
} 

#endif 