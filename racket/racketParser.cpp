#include "racketParser.hpp"
#include "scannerAdapter.hpp"
#include "firstFollow.hpp"
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

RacketParser::RacketParser(ScannerAdapter& scanner, bool useFirstFollow)
    : scanner(scanner), currentToken(TokenType::END_OF_FILE, "", 0, 0),
      firstFollowCalc(nullptr), useFirstFollowSets(useFirstFollow) {
    
    // Initialize FIRST/FOLLOW calculator if enabled
    if (useFirstFollowSets) {
        firstFollowCalc = new FirstFollowCalculator();
        firstFollowCalc->calculateFirst();
        firstFollowCalc->calculateFollow();
        
        std::cout << "Parser initialized with FIRST/FOLLOW sets" << std::endl;
    }
    
    // Initialize by getting the first token
    advance();
}

RacketParser::~RacketParser() {
    if (firstFollowCalc) {
        delete firstFollowCalc;
    }
}

const FirstFollowCalculator* RacketParser::getFirstFollowCalculator() const {
    return firstFollowCalc;
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
    
    // Use FIRST sets if enabled
    if (useFirstFollowSets && firstFollowCalc) {
        if (inFirst("list")) {
            return parseList();
        } else if (inFirst("literal")) {
            return parseLiteral();
        } else {
            error("Expected expression (current token not in FIRST(expr))");
        }
    }
    
    // Fallback to hardcoded checks
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
    
    // Determine what kind of form this is using FIRST sets
    std::unique_ptr<ExprNode> result;
    
    if (useFirstFollowSets && firstFollowCalc) {
        // Use FIRST sets to decide which production to use
        if (inFirst("quote")) {
            result = parseQuote();
        } else if (inFirst("if")) {
            result = parseIf();
        } else if (inFirst("begin")) {
            result = parseBegin();
        } else if (inFirst("app")) {
            result = parseApp();
        } else {
            // Implicit application (not in grammar, but supported)
            std::vector<std::unique_ptr<ExprNode>> exprs;
            
            while (!check(TokenType::RIGHT_PAREN) && !check(TokenType::RIGHT_BRACKET) && !isAtEnd()) {
                exprs.push_back(parseExpr());
            }
            
            if (exprs.empty()) {
                error("Application requires at least a function");
            }
            
            auto function = std::move(exprs[0]);
            std::vector<std::unique_ptr<ExprNode>> args;
            for (size_t i = 1; i < exprs.size(); ++i) {
                args.push_back(std::move(exprs[i]));
            }
            
            result = std::unique_ptr<ExprNode>(new AppNode(std::move(function), std::move(args)));
        }
    } else {
        // Fallback to hardcoded checks
        if (check(TokenType::QUOTE)) {
            result = parseQuote();
        } else if (check(TokenType::IF)) {
            result = parseIf();
        } else if (check(TokenType::BEGIN)) {
            result = parseBegin();
        } else if (check(TokenType::PLAIN_APP)) {
            result = parseApp();
        } else {
            // Implicit application
            std::vector<std::unique_ptr<ExprNode>> exprs;
            
            while (!check(TokenType::RIGHT_PAREN) && !check(TokenType::RIGHT_BRACKET) && !isAtEnd()) {
                exprs.push_back(parseExpr());
            }
            
            if (exprs.empty()) {
                error("Application requires at least a function");
            }
            
            auto function = std::move(exprs[0]);
            std::vector<std::unique_ptr<ExprNode>> args;
            for (size_t i = 1; i < exprs.size(); ++i) {
                args.push_back(std::move(exprs[i]));
            }
            
            result = std::unique_ptr<ExprNode>(new AppNode(std::move(function), std::move(args)));
        }
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

// ============================================================================
// FIRST/FOLLOW Helper Methods
// ============================================================================

std::string racket::RacketParser::tokenTypeToString(TokenType type) const {
    switch (type) {
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::DOT: return "DOT";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        case TokenType::BOOLEAN: return "BOOLEAN";
        case TokenType::QUOTE: return "QUOTE";
        case TokenType::IF: return "IF";
        case TokenType::BEGIN: return "BEGIN";
        case TokenType::PLAIN_APP: return "PLAIN_APP";
        case TokenType::LAMBDA: return "LAMBDA";
        case TokenType::LET_VALUES: return "LET_VALUES";
        case TokenType::LETREC_VALUES: return "LETREC_VALUES";
        case TokenType::SET: return "SET";
        case TokenType::CASE_LAMBDA: return "CASE_LAMBDA";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

bool racket::RacketParser::inFirst(const std::string& nonTerminal) const {
    if (!useFirstFollowSets || !firstFollowCalc) {
        return false;
    }
    
    auto firstSet = firstFollowCalc->getFirst(nonTerminal);
    std::string currentTokenStr = tokenTypeToString(currentToken.type);
    
    return firstSet.find(currentTokenStr) != firstSet.end();
}

bool racket::RacketParser::inFollow(const std::string& nonTerminal) const {
    if (!useFirstFollowSets || !firstFollowCalc) {
        return false;
    }
    
    auto followSet = firstFollowCalc->getFollow(nonTerminal);
    std::string currentTokenStr = tokenTypeToString(currentToken.type);
    
    return followSet.find(currentTokenStr) != followSet.end();
}

} // namespace racket

// Made with Bob
