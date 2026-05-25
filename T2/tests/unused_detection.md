# Sistema de Detecção de Variáveis/Funções Não Usadas

## Visão Geral
O transpilador agora implementa um sistema de avisos para detectar variáveis e funções que foram declaradas mas nunca foram usadas no código. Estes avisos não bloqueiam a compilação - o arquivo Python ainda é gerado.

## Como Funciona

### Rastreamento de Uso
1. Cada símbolo na tabela de símbolos possui um campo `foi_usado` que rastreia se foi referenciado
2. Quando uma variável é usada em uma expressão (ex: `x` em `(+ x 1)`), `marcar_como_usado()` é chamada
3. Após a compilação bem-sucedida, `verificar_nao_utilizados()` caminha pela tabela de símbolos

### Critérios para Avisos
Um aviso é gerado quando:
- Variável/função é declarada com `define`
- Nunca foi usada no código
- Não é uma função built-in (como `+`, `display`, etc)
- Tem número de linha de declaração > 0

### Rastreamento de Uso em Diferentes Contextos

#### Variáveis Simples
```scheme
(define x 5)    ; Declaração na linha 1
x               ; Uso - sem aviso
```

#### Variáveis em Expressões
```scheme
(define x 5)
(define y 10)
(+ x y)         ; Ambas usadas - sem aviso
```

#### Funções (Lambda)
```scheme
(define func1 (lambda (x) (+ x 1)))  ; Linha 1
(define func2 (lambda (x) (* x 2)))  ; Linha 2, nunca usada
(func1 5)       ; func1 usada
```
Resultado: Aviso para func2 apenas

#### Parâmetros de Lambda
```scheme
(define f (lambda (x y) (+ x y)))
```
Parâmetros `x` e `y` são locais ao lambda, marcados como usados quando referenciados no corpo

#### Parâmetros de Let
```scheme
(let ((x 5) (y 10)) (+ x y))
```
Parâmetros `x` e `y` são locais ao `let`, marcados como usados no corpo

## Exemplos de Saída

### Exemplo 1: Variáveis Não Usadas
Entrada:
```scheme
(define nao_usada 42)
(define x 5)
(define nao_usado 100)
(define y 10)
(+ x y)
```

Saída:
```
Convertendo Scheme para Python...
Arquivo 'saida.py' gerado com sucesso!

AVISO: Variavel/funcao 'nao_usado' declarada na linha 3 mas nunca usada
AVISO: Variavel/funcao 'nao_usada' declarada na linha 1 mas nunca usada

2 aviso(s) de variavel/funcao nao utilizada
```

### Exemplo 2: Funções Parcialmente Usadas
Entrada:
```scheme
(define soma (lambda (a b) (+ a b)))
(define mult_nao_usada (lambda (x y) (* x y)))
(define z 100)
(define w 200)
(soma 5 3)
z
```

Saída:
```
Convertendo Scheme para Python...
Arquivo 'saida.py' gerado com sucesso!

AVISO: Variavel/funcao 'w' declarada na linha 4 mas nunca usada
AVISO: Variavel/funcao 'mult_nao_usada' declarada na linha 2 mas nunca usada

2 aviso(s) de variavel/funcao nao utilizada
```

## Implementação Técnica

### Estruturas Modificadas
- `scheme.h`: Campo `foi_usado` adicionado ao `struct symrec`

### Funções Novas
- `marcar_como_usado(const char *name)`: Marca símbolo como usado
- `verificar_nao_utilizados(void)`: Gera avisos para símbolos não usados

### Pontos de Rastreamento
1. **Parser (sintatico.y)**:
   - `expressao: TOKEN_ID` - marca variáveis simples como usadas
   - `TOKEN_ID lista_argumentos` - marca função/operador como usado

2. **Inicialização (tabela_simbolos.c)**:
   - Built-ins marcados com `eh_builtin = 1` (não geram avisos)

3. **Verificação (tabela_simbolos.c)**:
   - Chamada em `main()` após `yyparse()` bem-sucedido

## Notas Importantes

1. **Avisos não bloqueiam compilação**: O arquivo Python é sempre gerado
2. **Built-ins não geram avisos**: Funções como `+`, `-`, `display`, etc nunca geram avisos
3. **Parâmetros de lambda/let locais**: Criados dinamicamente na tabela de símbolos
4. **Linhas reportadas**: Indicam exatamente onde a variável foi declarada
