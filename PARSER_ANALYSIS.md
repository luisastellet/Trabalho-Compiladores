
# Análise do Parser Racket - FIRST e FOLLOW

## Tipo de Parser Implementado

O parser atual é um **Recursive Descent Parser (Parser Descendente Recursivo)** que usa **lookahead de 1 token** (LL(1)).

## Como FIRST e FOLLOW são Usados

### Gramática Racket (Simplificada)

```
expr → literal | list
literal → IDENTIFIER | NUMBER | STRING | BOOLEAN
list → ( form )
form → quote-form | if-form | begin-form | app-form | implicit-app
```

### FIRST Sets (Implementados Implicitamente)

O parser usa `check(TokenType)` para verificar o FIRST de cada produção:

```cpp
// FIRST(expr) = { IDENTIFIER, NUMBER, STRING, BOOLEAN, LEFT_PAREN, LEFT_BRACKET }
std::unique_ptr<ExprNode> parseExpr() {
    if (check(TokenType::LEFT_PAREN) || check(TokenType::LEFT_BRACKET)) {
        return parseList();  // FIRST(list) = { LEFT_PAREN, LEFT_BRACKET }
    }
    return parseLiteral();   // FIRST(literal) = { IDENTIFIER, NUMBER, STRING, BOOLEAN }
}
```

```cpp
// FIRST(literal) explícito
std::unique_ptr<ExprNode> parseLiteral() {
    if (check(TokenType::IDENTIFIER)) {
        // FIRST(id) = { IDENTIFIER }
        return new IdNode(...);
    }
    if (check(TokenType::NUMBER)) {
        // FIRST(number) = { NUMBER }
        return new NumberNode(...);
    }
    if (check(TokenType::STRING)) {
        // FIRST(string) = { STRING }
        return new StringNode(...);
    }
    if (check(TokenType::BOOLEAN)) {
        // FIRST(boolean) = { BOOLEAN }
        return new BooleanNode(...);
    }
    error("Expected literal");
}
```

```cpp
// FIRST(form) - Decisão baseada no primeiro token após '('
std::unique_ptr<ExprNode> parseList() {
    consume(LEFT_PAREN);
    
    if (check(TokenType::QUOTE)) {
        // FIRST(quote-form) = { QUOTE }
        return parseQuote();
    } 
    else if (check(TokenType::IF)) {
        // FIRST(if-form) = { IF }
        return parseIf();
    } 
    else if (check(TokenType::BEGIN)) {
        // FIRST(begin-form) = { BEGIN }
        return parseBegin();
    } 
    else if (check(TokenType::PLAIN_APP)) {
        // FIRST(app-form) = { PLAIN_APP }
        return parseApp();
    } 
    else {
        // FIRST(implicit-app) = qualquer expr
        return parseImplicitApp();
    }
}
```

### FOLLOW Sets (Implementados Implicitamente)

O FOLLOW é usado para saber quando parar de parsear:

```cpp
// FOLLOW(expr) em diferentes contextos:

// 1. Em parseIf: FOLLOW(condition) = FIRST(then-expr)
//                FOLLOW(then-expr) = FIRST(else-expr)
//                FOLLOW(else-expr) = { RIGHT_PAREN }
std::unique_ptr<ExprNode> parseIf() {
    consume(IF);
    auto condition = parseExpr();  // Para quando encontra próximo token válido
