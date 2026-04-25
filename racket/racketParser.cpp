#include "racketParser.hpp"
#include "scannerAdapter.hpp"
#include <iostream>

namespace racket {

// ============================================================================
// Token Implementation
// ============================================================================

void Token::print() const {
    std::cout << "[" << line << ":" << column << "] ";
    
    switch (type) {
        case TokenType::LEFT_PAREN: std::cout << "LEFT_PAREN"; break;
        case TokenType::RIGHT_PAREN: std::cout << "RIGHT_PAREN"; break;
        case TokenType::LEFT_BRACKET: std::cout << "LEFT_BRACKET"; break;
        case TokenType::RIGHT_BRACKET: std::cout << "RIGHT_BRACKET"; break;
        case TokenType::DOT: std::cout << "DOT"; break;
        case TokenType::IDENTIFIER: std::cout << "IDENTIFIER"; break;
        case TokenType::NUMBER: std::cout << "NUMBER"; break;
        case TokenType::STRING: std::cout << "STRING"; break;
        case TokenType::BOOLEAN: std::cout << "BOOLEAN"; break;
        case TokenType::QUOTE: std::cout << "QUOTE"; break;
        case TokenType::IF: std::cout << "IF"; break;
        case TokenType::BEGIN: std::cout << "BEGIN"; break;
        case TokenType::PLAIN_APP: std::cout << "PLAIN_APP"; break;
        case TokenType::LAMBDA: std::cout << "LAMBDA"; break;
        case TokenType::LET_VALUES: std::cout << "LET_VALUES"; break;
        case TokenType::LETREC_VALUES: std::cout << "LETREC_VALUES"; break;
        case TokenType::SET: std::cout << "SET"; break;
        case TokenType::CASE_LAMBDA: std::cout << "CASE_LAMBDA"; break;
        case TokenType::END_OF_FILE: std::cout << "EOF"; break;
    }
    
    std::cout << " '" << lexeme << "'";
    
    if (type == TokenType::NUMBER) {
        std::cout << " (value: " << numValue << ")";
    }
    
    std::cout << std::endl;
}

// ============================================================================
// RacketParser Implementation
// ============================================================================

RacketParser::RacketParser(ScannerAdapter& scanner)
    : scanner(scanner), currentToken(TokenType::END_OF_FILE, "", 0, 0) {
    // Initialize by getting the first token
    advance();
}

std::unique_ptr<ExprNode> RacketParser::parse() {
    if (isAtEnd()) {
        error("Empty input");
    }
    
    auto expr = parseExpr();
    
    // Ensure we've consumed all input
    if (!isAtEnd()) {
        error("Unexpected tokens after expression");
    }
    
    return expr;
}

std::vector<std::unique_ptr<ExprNode>> RacketParser::parseMultiple() {
    std::vector<std::unique_ptr<ExprNode>> expressions;
    
    while (!isAtEnd()) {
        expressions.push_back(parseExpr());
    }
    
    return expressions;
}

// ============================================================================
// Token Management
// ============================================================================

void RacketParser::advance() {
    currentToken = scanner.nextToken();
}

bool racket::RacketParser::check(TokenType type) const {
    return currentToken.type == type;
}

bool racket::RacketParser::checkAny(const std::vector<TokenType>& types) const {
    for (TokenType type : types) {
        if (check(type)) {
            return true;
        }
    }
    return false;
}

void racket::RacketParser::consume(TokenType type, const std::string& message) {
    if (!check(type)) {
        error(message);
    }
    advance();
}

bool racket::RacketParser::isAtEnd() const {
    return check(TokenType::END_OF_FILE);
}

// ============================================================================
// Parsing Methods
// ============================================================================

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseExpr() {
    // expr = id | number | string | boolean | (...)
    
    if (check(TokenType::LEFT_PAREN) || check(TokenType::LEFT_BRACKET)) {
        return parseList();
    }
    
    return parseLiteral();
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseLiteral() {
    // Literal = id | number | string | boolean
    
    if (check(TokenType::IDENTIFIER)) {
        std::string name = currentToken.lexeme;
        advance();
        return std::unique_ptr<ExprNode>(new IdNode(name));
    }
    
    if (check(TokenType::NUMBER)) {
        double value = currentToken.numValue;
        advance();
        return std::unique_ptr<ExprNode>(new NumberNode(value));
    }
    
    if (check(TokenType::STRING)) {
        std::string value = currentToken.lexeme;
        advance();
        return std::unique_ptr<ExprNode>(new StringNode(value));
    }
    
    if (check(TokenType::BOOLEAN)) {
        bool value = (currentToken.lexeme == "#t");
        advance();
        return std::unique_ptr<ExprNode>(new BooleanNode(value));
    }
    
    error("Expected literal (identifier, number, string, or boolean)");
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseList() {
    // List = ( special-form | application )
    
    // Consume opening paren/bracket
    bool isSquare = check(TokenType::LEFT_BRACKET);
    if (isSquare) {
        consume(TokenType::LEFT_BRACKET, "Expected '['");
    } else {
        consume(TokenType::LEFT_PAREN, "Expected '('");
    }
    
    // Check for empty list (error for now)
    if (check(TokenType::RIGHT_PAREN) || check(TokenType::RIGHT_BRACKET)) {
        error("Empty list not supported in this grammar");
    }
    
    // Determine what kind of form this is
    std::unique_ptr<ExprNode> result;
    
    if (check(TokenType::QUOTE)) {
        result = parseQuote();
    } else if (check(TokenType::IF)) {
        result = parseIf();
    } else if (check(TokenType::BEGIN)) {
        result = parseBegin();
    } else if (check(TokenType::PLAIN_APP)) {
        result = parseApp();
    } else {
        // For now, treat any other list as an implicit application
        // In full Racket, this would be (#%plain-app ...)
        // We'll parse it as if #%plain-app was there
        std::vector<std::unique_ptr<ExprNode>> exprs;
        
        // Parse function and arguments
        while (!check(TokenType::RIGHT_PAREN) && !check(TokenType::RIGHT_BRACKET) && !isAtEnd()) {
            exprs.push_back(parseExpr());
        }
        
        if (exprs.empty()) {
            error("Application requires at least a function");
        }
        
        // First expr is the function, rest are arguments
        auto function = std::move(exprs[0]);
        std::vector<std::unique_ptr<ExprNode>> args;
        for (size_t i = 1; i < exprs.size(); ++i) {
            args.push_back(std::move(exprs[i]));
        }
        
        result = std::unique_ptr<ExprNode>(new AppNode(std::move(function), std::move(args)));
    }
    
    // Consume closing paren/bracket
    if (isSquare) {
        consume(TokenType::RIGHT_BRACKET, "Expected ']'");
    } else {
        consume(TokenType::RIGHT_PAREN, "Expected ')'");
    }
    
    return result;
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseQuote() {
    // (quote datum)
    
    consume(TokenType::QUOTE, "Expected 'quote'");
    
    auto datum = parseDatum();
    
    return std::unique_ptr<ExprNode>(new QuoteNode(std::move(datum)));
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseIf() {
    // (if expr expr expr)
    
    consume(TokenType::IF, "Expected 'if'");
    
    auto condition = parseExpr();
    auto thenBranch = parseExpr();
    auto elseBranch = parseExpr();
    
    return std::unique_ptr<ExprNode>(new IfNode(
        std::move(condition),
        std::move(thenBranch),
        std::move(elseBranch)
    ));
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseBegin() {
    // (begin expr ...+)
    
    consume(TokenType::BEGIN, "Expected 'begin'");
    
    std::vector<std::unique_ptr<ExprNode>> expressions;
    
    // Parse at least one expression
    expressions.push_back(parseExpr());
    
    // Parse remaining expressions
    while (!check(TokenType::RIGHT_PAREN) && !check(TokenType::RIGHT_BRACKET) && !isAtEnd()) {
        expressions.push_back(parseExpr());
    }
    
    return std::unique_ptr<ExprNode>(new BeginNode(std::move(expressions)));
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseApp() {
    // (#%plain-app expr ...+)
    
    consume(TokenType::PLAIN_APP, "Expected '#%plain-app'");
    
    // Parse function
    auto function = parseExpr();
    
    // Parse arguments
    std::vector<std::unique_ptr<ExprNode>> arguments;
    while (!check(TokenType::RIGHT_PAREN) && !check(TokenType::RIGHT_BRACKET) && !isAtEnd()) {
        arguments.push_back(parseExpr());
    }
    
    return std::unique_ptr<ExprNode>(new AppNode(std::move(function), std::move(arguments)));
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseDatum() {
    // For now, datum is just an expression
    // In full Racket, datum can be more complex (lists, vectors, etc.)
    // We'll handle that in future phases
    
    return parseExpr();
}

// ============================================================================
// Error Handling
// ============================================================================

void racket::RacketParser::error(const std::string& message) {
    throw ParseError(message, currentToken.line, currentToken.column);
}

void racket::RacketParser::synchronize() {
    // Skip tokens until we find a likely start of a new expression
    advance();
    
    while (!isAtEnd()) {
        // If we see a left paren, we might be at the start of a new expression
        if (check(TokenType::LEFT_PAREN) || check(TokenType::LEFT_BRACKET)) {
            return;
        }
        
        advance();
    }
}

} // namespace racket

// Made with Bob
