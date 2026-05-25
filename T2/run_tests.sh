#!/bin/bash

# =============================================================================
# SUITE DE TESTES DO TRANSPILADOR SCHEME → PYTHON
# =============================================================================
# Script centralizado para testar:
# - Detecção de variáveis/funções não declaradas
# - Parâmetros de lambda (devem ser declarados automaticamente)
# - Bindings de let/let*/letrec (devem ser declarados automaticamente)
# - Funções built-in (não devem gerar erros)
# - Geração de código bem-sucedida
# =============================================================================

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;36m'
NC='\033[0m' # No Color

# Contadores
TOTAL=0
PASSED=0
FAILED=0

# Helper function para exibir resultado de teste
run_test() {
    local test_name="$1"
    local scheme_code="$2"
    local grep_pattern="$3"  # Padrão para grep
    local should_fail="$4"   # "yes" se deve falhar (exit_code != 0), "no" se deve ter sucesso (exit_code == 0)

    TOTAL=$((TOTAL + 1))
    
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${BLUE}Teste #${TOTAL}:${NC} ${test_name}"
    echo -e "${BLUE}Código:${NC} ${scheme_code}"
    echo -e "${BLUE}Esperado:${NC} ${grep_pattern}"
    
    # Executa o transpilador (permite falha sem interromper script)
    set +e
    output=$(echo "$scheme_code" | ./transpilador 2>&1)
    exit_code=$?
    set -e
    
    # Verifica se a saída contém o padrão esperado
    if echo "$output" | grep -q "$grep_pattern"; then
        pattern_found=1
    else
        pattern_found=0
    fi
    
    # Verifica sucesso ou falha baseado na expectativa
    if [[ "$should_fail" == "yes" ]]; then
        # Esperamos erro (exit_code != 0) E o padrão de erro
        if [[ "$exit_code" != "0" ]] && [[ "$pattern_found" == "1" ]]; then
            echo -e "${GREEN}✓ PASSOU${NC}"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗ FALHOU${NC}"
            echo -e "  Exit code: ${exit_code} (esperado != 0)"
            echo -e "  Pattern encontrado: ${pattern_found} (esperado 1)"
            echo -e "  Output: ${output}"
            FAILED=$((FAILED + 1))
        fi
    else
        # Esperamos sucesso (exit_code == 0) E o padrão
        if [[ "$exit_code" == "0" ]] && [[ "$pattern_found" == "1" ]]; then
            echo -e "${GREEN}✓ PASSOU${NC}"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗ FALHOU${NC}"
            echo -e "  Exit code: ${exit_code} (esperado 0)"
            echo -e "  Pattern encontrado: ${pattern_found} (esperado 1)"
            echo -e "  Output: ${output}"
            FAILED=$((FAILED + 1))
        fi
    fi
    
    echo ""
}

# =============================================================================
# CATEGORIA 1: VARIÁVEIS NÃO DECLARADAS
# =============================================================================
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ CATEGORIA 1: VARIÁVEIS NÃO DECLARADAS                   ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

run_test \
    "Variável simples não declarada" \
    "(+ x 5)" \
    "ERRO: Variavel/funcao 'x' nao foi declarada" \
    "yes"

run_test \
    "Variável em expressão aninhada" \
    "(define y 10) (+ y (* x 2))" \
    "ERRO: Variavel/funcao 'x' nao foi declarada" \
    "yes"

run_test \
    "Múltiplas variáveis não declaradas" \
    "(+ x y z)" \
    "ERRO: Variavel/funcao" \
    "yes"

# =============================================================================
# CATEGORIA 2: VARIÁVEIS DECLARADAS (SUCESSO)
# =============================================================================
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ CATEGORIA 2: VARIÁVEIS DECLARADAS (SUCESSO)             ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

run_test \
    "Variável simples declarada e usada" \
    "(define y 10) (+ y 5)" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Múltiplas variáveis declaradas" \
    "(define a 5) (define b 10) (+ a b)" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Variável em expressão complexa" \
    "(define x 20) (* (+ x 5) (- x 3))" \
    "Arquivo.*saida.py" \
    "no"

# =============================================================================
# CATEGORIA 3: FUNÇÕES NÃO DECLARADAS
# =============================================================================
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ CATEGORIA 3: FUNÇÕES NÃO DECLARADAS                     ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

run_test \
    "Função não declarada - call simples" \
    "(undefined_func 1 2)" \
    "ERRO: Variavel/funcao 'undefined_func' nao foi declarada" \
    "yes"

run_test \
    "Função não declarada - em expressão" \
    "(define x 5) (my_func x)" \
    "ERRO: Variavel/funcao 'my_func' nao foi declarada" \
    "yes"

# =============================================================================
# CATEGORIA 4: LAMBDA - PARÂMETROS COMO PRÉ-DECLARADOS
# =============================================================================
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ CATEGORIA 4: LAMBDA (PARÂMETROS PRÉ-DECLARADOS)         ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

run_test \
    "Lambda simples com parâmetros" \
    "(define f (lambda (a b) (+ a b)))" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Lambda e aplicação com argumentos" \
    "(define f (lambda (a b) (+ a b))) (f 1 2)" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Lambda com parâmetros em operações" \
    "(define g (lambda (x y z) (* (+ x y) z)))" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Lambda aninhada" \
    "(define outer (lambda (a) (lambda (b) (+ a b))))" \
    "Arquivo.*saida.py" \
    "no"

# =============================================================================
# CATEGORIA 5: LET/LET*/LETREC - BINDINGS PRÉ-DECLARADOS
# =============================================================================
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ CATEGORIA 5: LET/LET*/LETREC (BINDINGS PRÉ-DECLARADOS)  ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

run_test \
    "Let simples com um binding" \
    "(let ((x 5)) (+ x 10))" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Let com múltiplos bindings" \
    "(let ((x 5) (y 10)) (+ x y))" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Let-star com referência entre bindings" \
    "(let* ((x 5) (y (+ x 10))) (+ x y))" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Letrec com função recursiva" \
    "(letrec ((fact (lambda (n) (if (= n 0) 1 (* n (fact (- n 1))))))) (fact 5))" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Let com binding não utilizado (aviso)" \
    "(let ((unused 5)) (+ 10 20))" \
    "Arquivo.*saida.py" \
    "no"

# =============================================================================
# CATEGORIA 6: FUNÇÕES BUILT-IN (SEM ERROS)
# =============================================================================
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ CATEGORIA 6: FUNÇÕES BUILT-IN (SEM ERROS)               ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

run_test \
    "Operador + (built-in)" \
    "(+ 1 2 3)" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Operador - (built-in)" \
    "(- 10 3)" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Operador * (built-in)" \
    "(* 5 4)" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Função list (built-in)" \
    "(list 1 2 3)" \
    "Arquivo.*saida.py" \
    "no"

# =============================================================================
# CATEGORIA 7: ESTRUTURAS DE CONTROLE
# =============================================================================
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ CATEGORIA 7: ESTRUTURAS DE CONTROLE                     ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

run_test \
    "If com variável declarada" \
    "(define x 5) (if (> x 3) (+ x 1) (- x 1))" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "If com variável não declarada" \
    "(if (> y 3) 10 20)" \
    "ERRO: Variavel/funcao 'y' nao foi declarada" \
    "yes"

run_test \
    "Cond com variável declarada" \
    "(define n 5) (cond ((= n 0) 1) ((> n 0) 2) (else 3))" \
    "Arquivo.*saida.py" \
    "no"

# =============================================================================
# CATEGORIA 8: CASOS MISTOS
# =============================================================================
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ CATEGORIA 8: CASOS MISTOS                               ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

run_test \
    "Função definida chamada com sucesso" \
    "(define square (lambda (x) (* x x))) (square 5)" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Função definida chamada com var undeclared" \
    "(define square (lambda (x) (* x x))) (square z)" \
    "ERRO: Variavel/funcao 'z' nao foi declarada" \
    "yes"

run_test \
    "Begin com múltiplas expressões" \
    "(define x 5) (begin (define y 10) (+ x y))" \
    "Arquivo.*saida.py" \
    "no"

run_test \
    "Set! com variável existente" \
    "(define counter 0) (set! counter 5) counter" \
    "Arquivo.*saida.py" \
    "no"

# =============================================================================
# RESUMO FINAL
# =============================================================================
echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}RESUMO DOS TESTES${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "Total:  ${TOTAL}"
echo -e "Passou: ${GREEN}${PASSED}${NC}"
echo -e "Falhou: ${RED}${FAILED}${NC}"
echo ""

if [[ $FAILED -eq 0 ]]; then
    echo -e "${GREEN}✓ TODOS OS TESTES PASSARAM!${NC}"
    exit 0
else
    echo -e "${RED}✗ ${FAILED} TESTE(S) FALHARAM${NC}"
    exit 1
fi
