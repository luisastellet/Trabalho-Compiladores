# Trabalho de Compiladores
Projeto da disciplina de Compiladores (prof. Bruno Lopes) — semestre 2026.1, UFF.

Autores: Alonso Bassini, Luisa Stellet e Miguel Jehle.

## Justificativa do uso da notação pós-fixada
Usamos notação pós-fixada para facilitar a construção de árvores sintáticas e reduzir a necessidade de lookahead na análise. Em pós-fixado, cada token é processado no fluxo: operandos são empilhados; operadores consomem seus operandos da pilha e empilham o resultado. Isso evita verificações complexas de precedência e associatividade em tempo de leitura.

## Observações de entrada
- Expressão regular em notação pós-fixada (postfix/RPN).
- Tokens separados por espaço em branco.
- Operadores:
  - `*` (fecho de Kleene)
  - `+` (uma ou mais ocorrências)
  - `?` (zero ou uma ocorrência)
  - `|` (alternância)
  - `.` (concatenação)

## Estrutura atual do código
- `syntaxTree.cpp` contém as classes básicas:
  - `Node`
  - `OperativeNodeUnary`
  - `OperativeNodeBinary`
  - `regularExpressionToken` (armazena expressão regular em uma árvore)

## TODO da syntaxTree
- Renomear as variáveis f, n, eps, nR, nL (final_state, number_of_states, alphabetSize, ... , ...)
- Transformar o NFA numa classe
- MUdar a struct do NFA para aceitar vários finais
- Diferenciar as classes NFA e DFA em arquivos diferentes (usar herança)

## Como executar o código
Compilar: ```make``` 

Executar: ```.\main\automaton.exe```