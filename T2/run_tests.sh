#!/bin/bash

# =============================================================================
# SUITE DE TESTES DO TRANSPILADOR SCHEME → PYTHON (VERBOSE)
# =============================================================================
# Executa testes usando arquivos .scheme da pasta 'cases/'
# Mostra todo o código, linha por linha
# Exibe a saída completa do transpilador
# Destaca claramente qual linha causou erro
# =============================================================================

set +e  # Permitir falhas para capturar exit codes

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;36m'
PURPLE='\033[0;35m'
GRAY='\033[0;37m'
NC='\033[0m' # No Color

# Contadores
TOTAL=0
PASSED=0
FAILED=0

# Helper: exibir número de linhas
show_code_with_lines() {
    local file="$1"
    awk 'NR {printf "%s%3d |%s %s\n", "'"${GRAY}"'", NR, "'"${NC}"'", $0}' "$file"
}

# Helper: mostrar comentário descritivo do teste
show_test_description() {
    local file="$1"
    grep "^; Teste:" "$file" | sed 's/; Teste: //'
    echo ""
}

# Executar um teste baseado em arquivo
run_file_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .scheme)
    local should_fail="$2"  # "yes" se deve falhar, "no" se deve compilar

    TOTAL=$((TOTAL + 1))
    
    echo ""
    echo -e "${YELLOW}╔════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║${NC} Teste #${TOTAL}: ${test_name}"                                ║
    echo -e "${YELLOW}╚════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    # Mostrar descrição e expectativa
    echo -e "${PURPLE} Descrição:${NC}"
    show_test_description "$test_file"
    
    # Mostrar código com números de linha
    echo -e "${BLUE} Código:${NC}"
    echo -e "${GRAY}────────────────────────────────────────────────────${NC}"
    show_code_with_lines "$test_file"
    echo -e "${GRAY}────────────────────────────────────────────────────${NC}"
    echo ""
    
    # Executar o transpilador
    output=$(./transpilador < "$test_file" 2>&1)
    exit_code=$?
    
    # Mostrar saída
    echo -e "${BLUE} Saída do Transpilador:${NC}"
    echo -e "${GRAY}────────────────────────────────────────────────────${NC}"
    
    # Destacar erros
    if echo "$output" | grep -q "ERRO"; then
        echo "$output" | while read line; do
            if echo "$line" | grep -q "ERRO"; then
                echo -e "${RED}${line}${NC}"
            else
                echo "$line"
            fi
        done
    else
        echo "$output"
    fi
    
    echo -e "${GRAY}────────────────────────────────────────────────────${NC}"
    echo ""
    
    # Avaliar resultado
    if [[ "$should_fail" == "yes" ]]; then
        # Esperamos erro
        if [[ "$exit_code" != "0" ]]; then
            echo -e "${GREEN} PASSOU${NC} (compilação falhou como esperado)"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED} FALHOU${NC} (esperava erro, mas compilou com sucesso)"
            FAILED=$((FAILED + 1))
        fi
    else
        # Esperamos sucesso
        if [[ "$exit_code" == "0" ]]; then
            echo -e "${GREEN} PASSOU${NC} (compilação bem-sucedida)"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED} FALHOU${NC} (compilação falhou inesperadamente)"
            FAILED=$((FAILED + 1))
        fi
    fi
    
    echo ""
}

# =============================================================================
# EXECUTAR TESTES DA PASTA 'cases/'
# =============================================================================

if [ ! -d "cases" ]; then
    echo -e "${RED}Erro: Pasta 'cases/' não encontrada!${NC}"
    exit 1
fi

echo -e "${YELLOW}╔════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║${NC}     SUITE DE TESTES - TRANSPILADOR SCHEME → PYTHON"           ║
echo -e "${YELLOW}║${NC}     Modo: VERBOSO com exibição de código e saída completa"    ║
echo -e "${YELLOW}╚════════════════════════════════════════════════════════════════════╝${NC}"

# Testes que DEVEM FALHAR (com erro)
echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${YELLOW}TESTES QUE DEVEM GERAR ERRO${NC}"
echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

run_file_test "cases/var_undeclared_simple.scheme" "yes"
run_file_test "cases/var_undeclared_nested.scheme" "yes"
run_file_test "cases/func_undeclared.scheme" "yes"
run_file_test "cases/complex_error.scheme" "yes"
run_file_test "cases/if_error.scheme" "yes"

# Testes que DEVEM COMPILAR COM SUCESSO
echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${YELLOW}TESTES QUE DEVEM COMPILAR COM SUCESSO${NC}"                       
echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

run_file_test "cases/lambda_ok.scheme" "no"
run_file_test "cases/let_multiple.scheme" "no"
run_file_test "cases/letstar_ok.scheme" "no"
run_file_test "cases/letrec_factorial.scheme" "no"
run_file_test "cases/complex_ok.scheme" "no"
run_file_test "cases/if_ok.scheme" "no"
run_file_test "cases/cond_ok.scheme" "no"

# =============================================================================
# RESUMO FINAL
# =============================================================================

echo ""
echo -e "${YELLOW}╔════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║${NC}                        RESUMO DOS TESTES"                     ║
echo -e "${YELLOW}╚════════════════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "Total de Testes:  ${BLUE}${TOTAL}${NC}"
echo -e "Testes Passando:  ${GREEN}${PASSED}${NC}"
echo -e "Testes Falhando:  ${RED}${FAILED}${NC}"
echo ""

if [[ $FAILED -eq 0 ]]; then
    echo -e "${GREEN}✓ TODOS OS TESTES PASSARAM!${NC}"
    echo ""
    exit 0
else
    echo -e "${RED}✗ ${FAILED} TESTE(S) FALHARAM${NC}"
    echo ""
    exit 1
fi
