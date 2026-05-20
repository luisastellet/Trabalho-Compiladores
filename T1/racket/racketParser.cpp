#include "racketParser.hpp"
#include "scannerAdapter.hpp"
#include "firstFollow.hpp"
#include <iostream>

namespace racket {

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


RacketParser::RacketParser(ScannerAdapter& scanner, bool useFirstFollow)
    : scanner(scanner), currentToken(TokenType::END_OF_FILE, "", 0, 0),
      firstFollowCalc(nullptr), useFirstFollowSets(useFirstFollow), errorCount(0),
      tokensFilename(scanner.getFilename()) {
    
    if (useFirstFollowSets) {
        firstFollowCalc = new FirstFollowCalculator();
        firstFollowCalc->calculateFirst();
        firstFollowCalc->calculateFollow();
        
        std::cout << "Parser inicializado com conjuntos FIRST/FOLLOW" << std::endl;
    }
    
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
        error("Entrada vazia");
    }
    
    auto expr = parseExpr();
    
    if (!expr) {
        error("Falha ao parsear expressao");
    }
    
    if (!isAtEnd()) {
        error("Tokens inesperados depois da expressao");
    }
    
    return expr;
}

std::vector<std::unique_ptr<ExprNode>> RacketParser::parseMultiple() {
    std::vector<std::unique_ptr<ExprNode>> expressions;

    while (!isAtEnd()) {
        int lineBefore   = currentToken.line;
        int columnBefore = currentToken.column;

        try {
            auto expr = parseExpr();
            if (expr != nullptr) {
                expressions.push_back(std::move(expr));
            } else {
                // parseExpr retornou nullptr, erro ja foi reportado.
                if (currentToken.line == lineBefore && currentToken.column == columnBefore) {
                    advance(); 
                }
            }
        } catch (const ParseError& e) {
            errorCount++;
            std::cerr << "\nErro sintatico [" << tokensFilename << ", linha " << e.line
                      << "]: " << e.what() << std::endl;
            synchronize();
        }
    }

    if (errorCount > 0) {
        std::cerr << "\nForam encontrados " << errorCount
                  << " erros sintaticos.\n" << std::endl;
    }

    return expressions;
}

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

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseExpr() {
    bool validStart = !(useFirstFollowSets && firstFollowCalc) || inFirst("expr");

    if (!validStart) {
        reportError("'" + currentToken.lexeme + "' nao pode iniciar uma expressao");
        bool canRetry = panicModeRecover("expr");
        if (!canRetry) return nullptr;
    }

    if (useFirstFollowSets && firstFollowCalc) {
        if (inFirst("list")) {
            return parseList();
        } else if (inFirst("literal")) {
            return parseLiteral();
        } else {
            reportError("'" + currentToken.lexeme + "' nao inicia nenhuma forma valida de expressao");
            return nullptr;
        }
    }

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

    // Celula vazia: nenhum literal reconhecido
    reportError("esperado identificador, numero, string ou booleano, mas encontrado '" + currentToken.lexeme + "'");
    return nullptr;
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseList() {
    // List = ( special-form | application )
    
    bool isSquare = check(TokenType::LEFT_BRACKET);
    if (isSquare) {
        consume(TokenType::LEFT_BRACKET, "Esperava '['");
    } else {
        consume(TokenType::LEFT_PAREN, "Esperava '('");
    }
    
    if (check(TokenType::RIGHT_PAREN) || check(TokenType::RIGHT_BRACKET)) {
        reportError("lista vazia '()' nao e permitida pela gramatica");
        advance();
        return nullptr;
    }
    
    std::unique_ptr<ExprNode> result;
    
    if (useFirstFollowSets && firstFollowCalc) {
        if (inFirst("quote")) {
            result = parseQuote();
        } else if (inFirst("if")) {
            result = parseIf();
        } else if (inFirst("begin")) {
            result = parseBegin();
        } else if (inFirst("app")) {
            result = parseApp();
        } else {
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
        if (check(TokenType::QUOTE)) {
            result = parseQuote();
        } else if (check(TokenType::IF)) {
            result = parseIf();
        } else if (check(TokenType::BEGIN)) {
            result = parseBegin();
        } else if (check(TokenType::PLAIN_APP)) {
            result = parseApp();
        } else {
            std::vector<std::unique_ptr<ExprNode>> exprs;
            
            while (!check(TokenType::RIGHT_PAREN) && !check(TokenType::RIGHT_BRACKET) && !isAtEnd()) {
                exprs.push_back(parseExpr());
            }
            
            if (exprs.empty()) {
                error("Aplicacao exige pelo menos uma funcao");
            }
            
            auto function = std::move(exprs[0]);
            std::vector<std::unique_ptr<ExprNode>> args;
            for (size_t i = 1; i < exprs.size(); ++i) {
                args.push_back(std::move(exprs[i]));
            }
            
            result = std::unique_ptr<ExprNode>(new AppNode(std::move(function), std::move(args)));
        }
    }
    
    if (isSquare) {
        consume(TokenType::RIGHT_BRACKET, "Esperava ']'");
    } else {
        consume(TokenType::RIGHT_PAREN, "Esperava ')'");
    }
    
    return result;
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseQuote() {
    consume(TokenType::QUOTE, "Esperava 'quote'");

    auto datum = parseDatum();

    return std::unique_ptr<ExprNode>(new QuoteNode(std::move(datum)));
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseIf() {
    consume(TokenType::IF, "Esperava 'if'");

    auto condition  = parseExpr();
    auto thenBranch = parseExpr();
    auto elseBranch = parseExpr();

    return std::unique_ptr<ExprNode>(new IfNode(
        std::move(condition),
        std::move(thenBranch),
        std::move(elseBranch)
    ));
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseBegin() {
    consume(TokenType::BEGIN, "Esperava 'begin'");

    std::vector<std::unique_ptr<ExprNode>> expressions;

    expressions.push_back(parseExpr());

    while (!check(TokenType::RIGHT_PAREN) && !check(TokenType::RIGHT_BRACKET) && !isAtEnd()) {
        expressions.push_back(parseExpr());
    }

    return std::unique_ptr<ExprNode>(new BeginNode(std::move(expressions)));
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseApp() {
    consume(TokenType::PLAIN_APP, "Esperava '#%plain-app'");

    auto function = parseExpr();

    std::vector<std::unique_ptr<ExprNode>> arguments;
    while (!check(TokenType::RIGHT_PAREN) && !check(TokenType::RIGHT_BRACKET) && !isAtEnd()) {
        arguments.push_back(parseExpr());
    }

    return std::unique_ptr<ExprNode>(new AppNode(std::move(function), std::move(arguments)));
}

std::unique_ptr<racket::ExprNode> racket::RacketParser::parseDatum() {
    return parseExpr();
}

// Lidando com Erros

void racket::RacketParser::error(const std::string& message) {
    throw ParseError(message, currentToken.line, currentToken.column);
}

void racket::RacketParser::reportError(const std::string& message) {
    errorCount++;
    std::cerr << "\nErro sintatico [" << tokensFilename << ", linha " << currentToken.line
              << "]: " << message << std::endl;
}

bool racket::RacketParser::panicModeRecover(const std::string& nonTerminal) {
    if (!firstFollowCalc) {
        return false;
    }
    std::set<std::string> firstSet  = firstFollowCalc->getFirst(nonTerminal);
    std::set<std::string> followSet = firstFollowCalc->getFollow(nonTerminal);

    auto inFirstSet = [&]() -> bool {
        return firstSet.count(tokenTypeToString(currentToken.type)) > 0;
    };
    auto inFollowSet = [&]() -> bool {
        return followSet.count(tokenTypeToString(currentToken.type)) > 0;
    };

    if (isAtEnd()) {
        std::cerr << "  --> Fim de entrada atingido. Analise encerrada." << std::endl;
        return false;
    }

    if (inFollowSet()) {
        std::cerr << "  --> Token '" << currentToken.lexeme << "' descartado." << std::endl;
        return false;
    }

    std::cerr << "  --> Descartando tokens ate encontrar ponto de sincronizacao: ";
    while (!isAtEnd()) {
        if (inFirstSet() || inFollowSet()) break;
        std::cerr << "'" << currentToken.lexeme << "' ";
        advance();
    }
    std::cerr << std::endl;

    if (isAtEnd() || inFollowSet()) {
        std::cerr << "  --> Continuando analise a partir de '" << currentToken.lexeme << "'." << std::endl;
        return false;
    }
    std::cerr << "  --> Retomando analise em '" << currentToken.lexeme << "'." << std::endl;
    return true;
}

void racket::RacketParser::synchronize() {
    // Salta tokens ate encontrar o inicio de uma nova expressão (FIRST(expr))
    while (!isAtEnd()) {
        if (useFirstFollowSets && firstFollowCalc) {
            if (inFirst("expr")) return;
        } else {
            if (check(TokenType::LEFT_PAREN) || check(TokenType::LEFT_BRACKET)) return;
        }
        advance();
    }
}

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
} 