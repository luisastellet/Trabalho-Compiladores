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

// Forward declarations
class ScannerAdapter;
class FirstFollowCalculator;

// Token Types

enum class TokenType {
    // Delimiters
    LEFT_PAREN,      // (
    RIGHT_PAREN,     // )
    LEFT_BRACKET,    // [
    RIGHT_BRACKET,   // ]
    DOT,             // .
    
    // Literals
    IDENTIFIER,      // x, foo, my-var
    NUMBER,          // 42, 3.14, -5
    STRING,          // "hello"
    BOOLEAN,         // #t, #f
    
    // Keywords
    QUOTE,           // quote
    IF,              // if
    BEGIN,           // begin
    PLAIN_APP,       // #%plain-app
    LAMBDA,          // #%plain-lambda
    LET_VALUES,      // let-values
    LETREC_VALUES,   // letrec-values
    SET,             // set!
    CASE_LAMBDA,     // case-lambda
    
    // Special
    END_OF_FILE      // EOF
};

// Token Structure

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    // For numeric literals
    double numValue;
    
    Token(TokenType type, const std::string& lexeme, int line, int column)
        : type(type), lexeme(lexeme), line(line), column(column), numValue(0.0) {}
    
    void print() const;
};

// Parser Exception

/**
 * @brief Exception thrown when parsing fails
 */
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

// Parser Class

/**
 * @brief Recursive descent parser for Racket
 * 
 * Parses tokens from the lexer and builds an Abstract Syntax Tree (AST).
 * Implements the following grammar (Phase 1):
 * 
 * expr = id
 *      | number
 *      | string
 *      | boolean
 *      | (quote datum)
 *      | (if expr expr expr)
 *      | (begin expr ...+)
 *      | (#%plain-app expr ...+)
 */
class RacketParser {
public:
    /**
     * @brief Construct a parser with a scanner adapter
     * @param scanner The scanner adapter to get tokens from
     * @param useFirstFollow Enable FIRST/FOLLOW based parsing (default: true)
     */
    explicit RacketParser(ScannerAdapter& scanner, bool useFirstFollow = true);
    
    /**
     * @brief Destructor
     */
    ~RacketParser();
    
    /**
     * @brief Parse the entire input and return the AST root
     * @return Unique pointer to the root expression node
     * @throws ParseError if parsing fails
     */
    std::unique_ptr<ExprNode> parse();
    
    /**
     * @brief Parse multiple expressions (for testing)
     * @return 
     */
    std::vector<std::unique_ptr<ExprNode>> parseMultiple();
    
    /**
     * @brief Get the FIRST/FOLLOW calculator (for inspection)
     */
    const FirstFollowCalculator* getFirstFollowCalculator() const;
    
    /**
     * @brief Verifica se algum erro foi encontrado durante a análise
     */
    bool hasErrors() const { return errorCount > 0; }
    
    /**
     * @brief Retorna o número total de erros encontrados
     */
    int getErrorCount() const { return errorCount; }
    
private:
    ScannerAdapter& scanner;
    Token currentToken;
    FirstFollowCalculator* firstFollowCalc;
    bool useFirstFollowSets;
    int errorCount;
    std::string tokensFilename;
    
    // Token management
    
    /**
     * @brief Advance to the next token
     */
    void advance();
    
    /**
     * @brief Check if current token matches expected type
     * @param type Expected token type
     * @return true if matches, false otherwise
     */
    bool check(TokenType type) const;
    
    /**
     * @brief Check if current token matches any of the expected types
     * @param types Vector of expected token types
     * @return true if matches any, false otherwise
     */
    bool checkAny(const std::vector<TokenType>& types) const;
    
    /**
     * @brief Consume current token if it matches expected type
     * @param type Expected token type
     * @param message Error message if doesn't match
     * @throws ParseError if token doesn't match
     */
    void consume(TokenType type, const std::string& message);
    
    /**
     * @brief Check if at end of input
     */
    bool isAtEnd() const;
    
    // Parsing methods
    
    /**
     * @brief Parse an expression
     * @return Unique pointer to expression node
     */
    std::unique_ptr<ExprNode> parseExpr();
    
    /**
     * @brief Parse a literal (id, number, string, boolean)
     * @return Unique pointer to expression node
     */
    std::unique_ptr<ExprNode> parseLiteral();
    
    /**
     * @brief Parse a list expression (special form or application)
     * @return Unique pointer to expression node
     */
    std::unique_ptr<ExprNode> parseList();
    
    /**
     * @brief Parse a quote expression: (quote datum)
     * @return Unique pointer to QuoteNode
     */
    std::unique_ptr<ExprNode> parseQuote();
    
    /**
     * @brief Parse an if expression: (if expr expr expr)
     * @return Unique pointer to IfNode
     */
    std::unique_ptr<ExprNode> parseIf();
    
    /**
     * @brief Parse a begin expression: (begin expr ...+)
     * @return Unique pointer to BeginNode
     */
    std::unique_ptr<ExprNode> parseBegin();
    
    /**
     * @brief Parse a function application: (#%plain-app expr ...+)
     * @return Unique pointer to AppNode
     */
    std::unique_ptr<ExprNode> parseApp();
    
    /**
     * @brief Parse datum for quote (simplified - just parse as expr for now)
     * @return Unique pointer to expression node
     */
    std::unique_ptr<ExprNode> parseDatum();
    
    // Error handling
    
    /**
     * @brief Lança erro de parse com localização do token atual
     * @param message Mensagem de erro
     * @throws ParseError
     */
    [[noreturn]] void error(const std::string& message);
    
    /**
     * @brief Reporta um erro sem lancar excecao
     */
    void reportError(const std::string& message);
    
    /**
     * @brief Recuperação em Modo Pânico para o não-terminal A.
     *
     * Algoritmo:
     *  - Se token atual é EOF ou pertence a FOLLOW(A) → descarta A (retorna false)
     *  - Caso contrário → salta tokens até FIRST(A) ∪ FOLLOW(A)
     *    - Se parou em FIRST(A) → retorna true (caller pode tentar parsear A)
     *    - Se parou em FOLLOW(A) ou EOF → retorna false (desempilha A)
     *
     * @param nonTerminal Nome do não-terminal em recuperação
     * @return true se o parser pode tentar parsear o símbolo novamente; false para desempilhar
     */
    bool panicModeRecover(const std::string& nonTerminal);
    
    /**
     * @brief Avança tokens até encontrar o início de uma nova expressão (FIRST(expr))
     */
    void synchronize();
    
    // FIRST/FOLLOW based parsing helpers
    
    /**
     * @brief Checa se o token atual pertence ao conjunto FIRST de um não-terminal
     * @param nonTerminal Nome do não-terminal a ser verificado
     * @return true se o token atual está em FIRST(nonTerminal)
     */
    bool inFirst(const std::string& nonTerminal) const;
    
    /**
     * @brief Checa se o token atual pertence ao conjunto FOLLOW de um não-terminal
     * @param nonTerminal Nome do não-terminal a ser verificado
     * @return true se o token atual está em FOLLOW(nonTerminal)
     */
    bool inFollow(const std::string& nonTerminal) const;
    
    /**
     * @brief Converte TokenType para string para lookup em FIRST/FOLLOW
     */
    std::string tokenTypeToString(TokenType type) const;
};

} 

#endif 