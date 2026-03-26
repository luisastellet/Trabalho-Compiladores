# Trabalho de Compiladores
Projeto da disciplina de Compiladores (prof. Bruno Lopes) — semestre 2026.1, UFF.

Autores: Alonso Bassini, Luisa Stellet e Miguel Jehle.

## Motivação
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
- As entradas agora podem ou não terminar com espaço; o parser considera o token residual na última posição.

## Estrutura atual do código
- `syntaxTree.cpp` contém as classes básicas:
  - `Node`
  - `OperativeNodeUnary`
  - `OperativeNodeBinary`
  - `regularExpressionToken` (construção de árvore)

## Próximos passos
- Refatorar classes em arquivos separados (modularização)
- Ajustar controle de memória (pointers -> smart pointers)
- Validar todos os casos de entrada inválidos e reportar erros
- Implementar impressão ou travessia de árvore para depuração
