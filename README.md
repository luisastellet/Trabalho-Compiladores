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
- Refatorar classes em arquivos separados (modularização)
- Imprimir a árvore sintática para debug
- Criação de método de leitura dos arquivos contendo as expressões regulares