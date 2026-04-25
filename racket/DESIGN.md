# Design do Parser Racket

## Visão Geral
Implementação de um parser para um subconjunto da gramática Racket Core Forms, usando análise sintática descendente recursiva.

## BNF Racket (Subconjunto Inicial)

### Expressões Básicas (Fase 1)
```
expr = id
     | (quote datum)
     | (if expr expr expr)
     | (begin expr ...+)
     | (#%plain-app expr ...+)
```

### Expansão Futura (Fase 2)
```
expr = ... (fase 1)
     | (#%plain-lambda formals expr ...+)
     | (case-lambda (formals expr ...+) ...)
     | (let-values ([(id ...) expr] ...) expr ...+)
     | (letrec-values ([(id ...) expr] ...) expr ...+)
     | (set! id expr)

formals = (id ...)
        | (id ...+ . id)
        | id
```

## Estrutura de Tokens

### Tipos de Token
```cpp
enum TokenType {
    // Delimitadores
    LPAREN,          // (
    RPAREN,          // )
    LBRACKET,        // [
    RBRACKET,        // ]
    DOT,             // .
    
    // Literais
    IDENTIFIER,      // x, foo, my-var
    NUMBER,          // 42, 3.14
    STRING,          // "hello"
    BOOLEAN,         // #t, #f
    
    // Keywords especiais
    QUOTE,           // quote
    IF,              // if
    BEGIN,           // begin
    LAMBDA,          // #%plain-lambda
    APP,             // #%plain-app
    LET_VALUES,      // let-values
    LETREC_VALUES,   // letrec-values
    SET,             // set!
    CASE_LAMBDA,     // case-lambda
    
    // Controle
    EOF_TOKEN,
    ERROR
};
```

### Regras Léxicas
- **Identificadores**: Começam com letra ou símbolos especiais (`-`, `+`, `*`, `/`, `?`, `!`, etc.)
  - Podem conter letras, dígitos, hífens, underscores
  - Exemplos: `x`, `my-var`, `foo?`, `set!`
  
- **Números**: Inteiros ou decimais
  - Exemplos: `42`, `-17`, `3.14`, `-2.5`
  
- **Strings**: Entre aspas duplas
  - Exemplo: `"hello world"`
  
- **Booleanos**: `#t` (true) ou `#f` (false)

- **Comentários**: Começam com `;` até o fim da linha

## Estrutura da AST

### Hierarquia de Classes

```
ASTNode (abstrata)
├── ExprNode (abstrata)
│   ├── IdNode              // identificador
│   ├── NumberNode          // literal numérico
│   ├── StringNode          // literal string
│   ├── BooleanNode         // #t ou #f
│   ├── QuoteNode           // (quote datum)
│   ├── IfNode              // (if cond then else)
│   ├── BeginNode           // (begin expr ...)
│   ├── AppNode             // (#%plain-app func args...)
│   ├── LambdaNode          // (#%plain-lambda formals body...)
│   ├── LetValuesNode       // (let-values bindings body...)
│   └── SetNode             // (set! id expr)
└── FormalsNode             // formals de lambda
```

### Exemplo de AST

Código Racket:
```racket
(if (quote #t)
    (begin 1 2 3)
    42)
```

AST:
```
IfNode
├── condition: QuoteNode
│   └── datum: BooleanNode(#t)
├── thenBranch: BeginNode
│   ├── expr[0]: NumberNode(1)
│   ├── expr[1]: NumberNode(2)
│   └── expr[2]: NumberNode(3)
└── elseBranch: NumberNode(42)
```

## Estratégia de Parsing

### Parser Recursivo Descendente

Cada produção da gramática corresponde a um método:

```cpp
class RacketParser {
    ExprNode* parseExpr();
    ExprNode* parseQuote();
    ExprNode* parseIf();
    ExprNode* parseBegin();
    ExprNode* parseApp();
    // ... outros métodos
};
```

### Algoritmo de Parsing

1. **parseExpr()**: Ponto de entrada principal
   - Se token atual é ID → retorna IdNode
   - Se token atual é NUMBER → retorna NumberNode
   - Se token atual é STRING → retorna StringNode
   - Se token atual é LPAREN → analisa forma especial
     - Consome `(`
     - Verifica próximo token (keyword)
     - Chama método específico (parseIf, parseQuote, etc.)
     - Consome `)`

2. **parseIf()**:
   ```
   (if <condition> <then> <else>)
   ```
   - Já consumiu `(` e `if`
   - Parse condition: `parseExpr()`
   - Parse then: `parseExpr()`
   - Parse else: `parseExpr()`
   - Consome `)`
   - Retorna IfNode

3. **parseBegin()**:
   ```
   (begin <expr> ...+)
   ```
   - Já consumiu `(` e `begin`
   - Loop: enquanto não encontrar `)`
     - Parse expr: `parseExpr()`
     - Adiciona à lista
   - Consome `)`
   - Retorna BeginNode

## Tratamento de Erros

### Tipos de Erro
1. **Erro Léxico**: Token inválido
2. **Erro Sintático**: Estrutura inválida
3. **Erro Semântico**: (fase futura)

### Estratégia
- Lançar exceções com mensagens descritivas
- Incluir posição (linha, coluna) do erro
- Modo de recuperação: sincronizar no próximo `)`

## Exemplos de Teste

### Teste 1: Identificador simples
```racket
x
```
AST: `IdNode("x")`

### Teste 2: Quote
```racket
(quote hello)
```
AST: `QuoteNode(IdNode("hello"))`

### Teste 3: If
```racket
(if #t 1 2)
```
AST: `IfNode(BooleanNode(true), NumberNode(1), NumberNode(2))`

### Teste 4: Begin
```racket
(begin 1 2 3)
```
AST: `BeginNode([NumberNode(1), NumberNode(2), NumberNode(3)])`

### Teste 5: Aplicação de função
```racket
(#%plain-app + 1 2)
```
AST: `AppNode(IdNode("+"), [NumberNode(1), NumberNode(2)])`

### Teste 6: Composição
```racket
(if (quote #t)
    (begin 
      (#%plain-app print "hello")
      42)
    0)
```

## Fases de Implementação

### Fase 1: Fundação (Atual)
- [x] Análise da BNF
- [ ] Estrutura de pastas
- [ ] Definição de tokens
- [ ] Classes da AST
- [ ] Lexer básico
- [ ] Parser para subconjunto básico

### Fase 2: Expansão
- [ ] Lambda e formals
- [ ] Let-values e letrec-values
- [ ] Case-lambda
- [ ] Set!

### Fase 3: Refinamento
- [ ] Tratamento robusto de erros
- [ ] Mensagens de erro detalhadas
- [ ] Otimizações
- [ ] Testes extensivos

## Referências
- Racket Reference: https://docs.racket-lang.org/reference/
- BNF fornecida: Racket Core Forms