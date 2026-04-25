# Plano de Implementação - Parser Racket

## Resumo Executivo

Implementação de um parser para a linguagem Racket usando a BNF oficial do Racket Core Forms. O projeto será desenvolvido em C++ de forma incremental, começando com um subconjunto básico de expressões e expandindo gradualmente.

## Decisões Arquiteturais

### ✅ Decisões Tomadas
1. **Linguagem**: C++ (consistência com código existente)
2. **Abordagem**: Incremental - começar simples e expandir
3. **Estrutura**: Pasta `racket/` dedicada com lexer, parser e AST separados
4. **Método**: Parser recursivo descendente (Recursive Descent)
5. **Subconjunto Inicial**: `id`, `quote`, `if`, `begin`, `#%plain-app`

### 📋 BNF Utilizada
```
expr = id
     | (quote datum)
     | (if expr expr expr)
     | (begin expr ...+)
     | (#%plain-app expr ...+)
```

## Roadmap de Implementação

### 🎯 Fase 1: Fundação (Semana 1)
**Objetivo**: Parser funcional para subconjunto básico

#### Tarefas
- [x] Análise e documentação da BNF
- [ ] Criar estrutura de pastas `racket/`
- [ ] Implementar [`racketAST.hpp`](racket/racketAST.hpp) - Classes da AST
- [ ] Implementar [`racketLexer.hpp/cpp`](racket/racketLexer.hpp) - Tokenização
- [ ] Implementar [`racketParser.hpp/cpp`](racket/racketParser.hpp) - Parsing
- [ ] Criar [`tests/racket_test.rkt`](tests/racket_test.rkt) - Casos de teste
- [ ] Criar [`main/racketMain.cpp`](main/racketMain.cpp) - Programa de teste
- [ ] Atualizar [`Makefile`](Makefile)

#### Entregáveis
- Parser funcional para expressões básicas
- Suite de testes básica
- Documentação inicial

### 🚀 Fase 2: Expansão (Semana 2)
**Objetivo**: Adicionar construções mais complexas

#### Tarefas
- [ ] Implementar `#%plain-lambda` e `formals`
- [ ] Implementar `let-values` e `letrec-values`
- [ ] Implementar `case-lambda`
- [ ] Implementar `set!`
- [ ] Expandir suite de testes

#### Entregáveis
- Parser completo para gramática expandida
- Testes para todas as construções

### 🔧 Fase 3: Refinamento (Semana 3)
**Objetivo**: Robustez e qualidade

#### Tarefas
- [ ] Tratamento robusto de erros
- [ ] Mensagens de erro descritivas
- [ ] Otimizações de performance
- [ ] Documentação completa
- [ ] Testes extensivos

#### Entregáveis
- Parser robusto e otimizado
- Documentação completa
- Suite de testes abrangente

## Estrutura de Arquivos

```
Trabalho-Compiladores/
├── racket/                    # Nova pasta
│   ├── DESIGN.md             # ✅ Documento de design
│   ├── ARCHITECTURE.md       # ✅ Arquitetura do sistema
│   ├── PLAN.md              # ✅ Este arquivo
│   ├── racketAST.hpp        # Classes da AST
│   ├── racketAST.cpp        # Implementação da AST
│   ├── racketLexer.hpp      # Interface do lexer
│   ├── racketLexer.cpp      # Implementação do lexer
│   ├── racketParser.hpp     # Interface do parser
│   └── racketParser.cpp     # Implementação do parser
├── tests/
│   ├── racket_test.rkt      # Exemplos de código Racket
│   └── racket_expected.txt  # ASTs esperadas
├── main/
│   └── racketMain.cpp       # Programa principal
└── Makefile                 # Atualizado para compilar Racket

Arquivos existentes (mantidos):
├── automata/                # DFA/NFA (Parte 1)
├── syntax/                  # Árvore sintática regex (Parte 1)
├── scanner.c               # Scanner (Parte 1)
└── README.md               # Atualizar com info do parser
```

## Componentes Principais

### 1. Lexer (racketLexer.hpp/cpp)
**Responsabilidade**: Converter código fonte em tokens

**Tokens Suportados**:
- Delimitadores: `(`, `)`, `[`, `]`, `.`
- Literais: identificadores, números, strings, booleanos
- Keywords: `quote`, `if`, `begin`, `#%plain-app`, etc.

**Interface**:
```cpp
class RacketLexer {
public:
    RacketLexer(const string& source);
    Token nextToken();
    Token peek();
    bool hasNext();
};
```

### 2. Parser (racketParser.hpp/cpp)
**Responsabilidade**: Construir AST a partir dos tokens

**Métodos Principais**:
```cpp
class RacketParser {
public:
    RacketParser(RacketLexer& lexer);
    ExprNode* parse();
    
private:
    ExprNode* parseExpr();
    ExprNode* parseQuote();
    ExprNode* parseIf();
    ExprNode* parseBegin();
    ExprNode* parseApp();
};
```

### 3. AST (racketAST.hpp/cpp)
**Responsabilidade**: Representar estrutura do programa

**Classes Principais**:
- `ASTNode` (base abstrata)
- `ExprNode` (expressões)
- `IdNode`, `NumberNode`, `StringNode`, `BooleanNode` (literais)
- `QuoteNode`, `IfNode`, `BeginNode`, `AppNode` (formas especiais)

## Exemplos de Uso

### Exemplo 1: Expressão Simples
```racket
42
```
**AST**: `NumberNode(42)`

### Exemplo 2: If Condicional
```racket
(if #t 1 0)
```
**AST**:
```
IfNode
├── condition: BooleanNode(true)
├── thenBranch: NumberNode(1)
└── elseBranch: NumberNode(0)
```

### Exemplo 3: Begin com Múltiplas Expressões
```racket
(begin 
  (#%plain-app print "hello")
  42)
```
**AST**:
```
BeginNode
├── expressions[0]: AppNode
│   ├── function: IdNode("print")
│   └── arguments[0]: StringNode("hello")
└── expressions[1]: NumberNode(42)
```

## Critérios de Sucesso

### ✅ Fase 1 Completa Quando:
- [ ] Parser compila sem erros
- [ ] Todos os testes básicos passam
- [ ] AST é gerada corretamente para exemplos simples
- [ ] Código está documentado

### ✅ Fase 2 Completa Quando:
- [ ] Todas as construções da BNF expandida funcionam
- [ ] Testes cobrem todos os casos
- [ ] Exemplos complexos são parseados corretamente

### ✅ Fase 3 Completa Quando:
- [ ] Erros são tratados graciosamente
- [ ] Performance é aceitável
- [ ] Documentação está completa
- [ ] Código está pronto para produção

## Próximos Passos

### Imediato (Agora)
1. Revisar e aprovar este plano
2. Mudar para modo Code
3. Começar implementação da Fase 1

### Após Aprovação
1. Criar arquivos base da AST
2. Implementar lexer básico
3. Implementar parser para subconjunto
4. Criar testes iniciais
5. Validar funcionamento

## Recursos Necessários

### Conhecimento
- C++ (classes, herança, ponteiros)
- Teoria de compiladores (lexer, parser)
- Gramáticas BNF
- Racket (sintaxe básica)

### Ferramentas
- Compilador C++ (g++)
- Make
- Editor de texto
- Git (controle de versão)

## Riscos e Mitigações

| Risco | Impacto | Mitigação |
|-------|---------|-----------|
| BNF muito complexa | Alto | Abordagem incremental |
| Bugs no parser | Médio | Testes extensivos |
| Performance ruim | Baixo | Otimizar depois |
| Escopo cresce | Médio | Manter foco no subconjunto |

## Referências

- [DESIGN.md](racket/DESIGN.md) - Design detalhado
- [ARCHITECTURE.md](racket/ARCHITECTURE.md) - Arquitetura do sistema
- [Racket Reference](https://docs.racket-lang.org/reference/) - Documentação oficial
- BNF fornecida - Racket Core Forms

---

**Status**: ✅ Planejamento completo - Pronto para implementação

**Última atualização**: 2026-04-25