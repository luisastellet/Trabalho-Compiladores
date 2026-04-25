# Trabalho de Compiladores

Implementação de um compilador completo com gerador de scanner, scanner léxico e parser sintático para a linguagem Racket.

## 🎯 Arquitetura do Sistema

```
┌─────────────────┐
│  Gerador de     │
│  Scanner (C++)  │  ← tests/racket_regex.txt
└────────┬────────┘
         │ gera
         ▼
┌─────────────────┐
│  scanner.c (C)  │
└────────┬────────┘
         │ processa
         ▼
┌─────────────────┐
│  tokens.txt     │  ← tests/test_input.txt
└────────┬────────┘
         │ lê
         ▼
┌─────────────────┐
│  Parser Racket  │
│  (C++)          │
└────────┬────────┘
         │ gera
         ▼
┌─────────────────┐
│  AST (Árvore    │
│  Sintática)     │
└─────────────────┘
```

## 🚀 Uso Rápido

### Build e Teste Completo

**Modo Normal (saída limpa):**
```bash
./build_and_run.sh
```

**Modo Verbose (mostra autômatos e detalhes de debug):**
```bash
./build_and_run.sh -v
# ou
./build_and_run.sh --verbose
```

Este script executa todo o fluxo:
1. ✅ Compila o gerador de scanner
2. ✅ Gera o scanner.c
3. ✅ Compila o scanner
4. ✅ Gera tokens.txt
5. ✅ Compila o parser
6. ✅ Executa testes e mostra AST

### Comandos Individuais

```bash
# Compilar tudo
make all

# Compilar apenas o gerador
make automaton

# Compilar apenas o parser
make racket

# Gerar tokens de um arquivo
./scanner input.txt

# Parsear tokens e gerar AST
./main/racket_parser tokens.txt

# Parsear múltiplas expressões
./main/racket_parser -m tokens.txt

# Mostrar tokens lidos
./main/racket_parser -l tokens.txt

# Mostrar FIRST/FOLLOW sets
./main/racket_parser -f
```

## 📁 Estrutura do Projeto

```
.
├── build_and_run.sh          # Script principal do fluxo completo
├── Makefile                  # Build system
│
├── main/
│   ├── automaton.cpp         # Gerador de scanner
│   └── racketMain.cpp        # Programa principal do parser
│
├── automata/
│   ├── nfa.cpp/hpp          # Autômato Finito Não-Determinístico
│   └── dfa.cpp/hpp          # Autômato Finito Determinístico
│
├── syntax/
│   └── syntaxTree.cpp/hpp   # Árvore sintática para regex
│
├── racket/
│   ├── racketParser.cpp/hpp      # Parser LL(1)
│   ├── racketAST.cpp/hpp         # Nós da AST
│   ├── firstFollow.cpp/hpp       # Cálculo de FIRST/FOLLOW
│   └── scannerAdapter.cpp/hpp    # Adaptador tokens.txt → parser
│
└── tests/
    ├── racket_regex.txt     # Definições de tokens (regex)
    └── test_input.txt       # Código Racket de teste
```

## 🔧 Tecnologias

- **Gerador de Scanner**: C++ (NFA → DFA → Minimização)
- **Scanner**: C (gerado automaticamente)
- **Parser**: C++ (Recursive Descent, LL(1))
- **Formato de Tokens**: Arquivo texto intermediário

## 📝 Sintaxe das Regex

O gerador usa **notação polonesa reversa (posfixa)**:

```
i f .                    # "if" (concatenação)
[a-z] [a-z] * .         # identificador (letra + zero ou mais letras)
[0-9] [0-9] * .         # número (dígito + zero ou mais dígitos)
" [a-z] * . " .         # string ("..." com letras)
```

**Operadores:**
- `.` = concatenação (binário)
- `|` = alternância/ou (binário)
- `*` = zero ou mais (unário)
- `+` = uma ou mais (unário)
- `?` = zero ou um (unário)
- `[a-z]` = classe de caracteres

## 🎓 Características do Parser

- **Tipo**: Recursive Descent (Top-Down)
- **Gramática**: LL(1)
- **FIRST/FOLLOW**: Calculados algoritmicamente
- **Suporte**: Múltiplas expressões
- **AST**: Árvore sintática abstrata completa

### Nós da AST

- `IfNode`: Expressões condicionais
- `AppNode`: Aplicação de função
- `IdNode`: Identificadores
- `NumberNode`: Números
- `StringNode`: Strings
- `BooleanNode`: Booleanos

## ✅ Exemplo de Uso

**Entrada** (`tests/test_input.txt`):
```racket
(if x y z)
(begin a b c)
(quote foo)
```

**Saída** (AST):
```
Expression 1:
  IfNode:
    condition: IdNode: x
    then: IdNode: y
    else: IdNode: z

Expression 2:
  AppNode:
    function: IdNode: begin
    arguments: (3)
      [0]: IdNode: a
      [1]: IdNode: b
      [2]: IdNode: c

Expression 3:
  AppNode:
    function: IdNode: quote
    arguments: (1)
      [0]: IdNode: foo
```

## 🧹 Limpeza

```bash
make clean  # Remove binários e arquivos gerados
```

## 📚 Documentação Adicional

- `INTEGRATION.md` - Detalhes da integração scanner/parser
- `racket/DESIGN.md` - Design do parser
- `racket/ARCHITECTURE.md` - Arquitetura do sistema
- `racket/PLAN.md` - Plano de implementação

---

**Desenvolvido para a disciplina de Compiladores - UFF**