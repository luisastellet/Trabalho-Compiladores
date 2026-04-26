#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR"

# Parse command line arguments
VERBOSE=false
for arg in "$@"; do
    case $arg in
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  -v, --verbose    Modo verboso (mostra autômatos e detalhes)"
            echo "  -h, --help       Mostra esta mensagem"
            exit 0
            ;;
    esac
done

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}      Trabalho de Compiladores - Fluxo Completo${NC}"
echo -e "${BLUE}      Gerador → Scanner → tokens.txt → Parser → AST${NC}"
if [ "$VERBOSE" = true ]; then
    echo -e "${BLUE}      Modo: VERBOSE (mostrando detalhes de debug)${NC}"
else
    echo -e "${BLUE}      Modo: NORMAL${NC}"
fi
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

echo -e "${YELLOW}[1/6] Limpando builds anteriores...${NC}"
cd "$BUILD_DIR"
rm -f main/automaton main/racket_parser scanner scanner.c tokens.txt
echo -e "${GREEN}✓ Limpeza concluída${NC}\n"

echo -e "${YELLOW}[2/6] Compilando Gerador de Scanner (C++)...${NC}"
make automaton

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Erro na compilação do gerador${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Gerador compilado${NC}\n"

echo -e "${YELLOW}[3/6] Gerando Scanner (scanner.c)...${NC}"
if [ "$VERBOSE" = true ]; then
    echo -e "${BLUE}Modo verboso: Mostrando construção dos autômatos...${NC}\n"
    ./main/automaton tests/racket_regex.txt
else
    ./main/automaton tests/racket_regex.txt > /dev/null 2>&1
fi

if [ ! -f "$BUILD_DIR/scanner.c" ]; then
    echo -e "${RED}✗ Erro: scanner.c não foi gerado${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Scanner gerado (scanner.c)${NC}\n"

echo -e "${YELLOW}[4/6] Compilando Scanner (C)...${NC}"
gcc -Wall -o scanner scanner.c

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Erro na compilação do scanner${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Scanner compilado${NC}\n"

echo -e "${YELLOW}[5/6] Executando Scanner para gerar tokens.txt...${NC}"
if [ "$VERBOSE" = true ]; then
    echo -e "${BLUE}Modo verboso: Mostrando análise léxica...${NC}\n"
    ./scanner tests/test_input.txt
else
    ./scanner tests/test_input.txt > /dev/null 2>&1
fi

if [ ! -f "$BUILD_DIR/tokens.txt" ]; then
    echo -e "${RED}✗ Erro: tokens.txt não foi gerado${NC}"
    exit 1
fi
echo -e "${GREEN}✓ tokens.txt gerado ($(wc -l < tokens.txt | xargs) tokens)${NC}\n"

echo -e "${YELLOW}[6/6] Compilando Parser Racket (C++)...${NC}"
make racket

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Erro na compilação do parser${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Parser compilado${NC}\n"

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}                    Executando Testes${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

echo -e "${YELLOW}━━━ Teste 1: Tokens Gerados ━━━${NC}"
if [ "$VERBOSE" = true ]; then
    echo -e "${BLUE}Arquivo:${NC} tokens.txt ($(wc -l < tokens.txt | xargs) linhas)"
    echo -e "${BLUE}Conteúdo completo:${NC}"
    cat tokens.txt
else
    echo -e "${BLUE}Arquivo:${NC} tokens.txt ($(wc -l < tokens.txt | xargs) linhas)"
    head -6 tokens.txt
    echo -e "${BLUE}...${NC}"
fi
echo ""

echo -e "${YELLOW}━━━ Teste 2: Parser Gerando AST ━━━${NC}"
./main/racket_parser -m tokens.txt
echo ""

echo -e "${YELLOW}━━━ Teste 3: Análise FIRST/FOLLOW ━━━${NC}"
./main/racket_parser -f
echo ""

echo -e "${GREEN}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${GREEN}      ✓ Build e testes concluídos com sucesso!${NC}"
echo -e "${GREEN}═══════════════════════════════════════════════════════════════${NC}\n"

echo -e "${BLUE}Fluxo Completo:${NC}"
echo -e "  1. ${YELLOW}Gerador de Scanner${NC} (automato) + regex → gera scanner.c"
echo -e "  2. ${YELLOW}Scanner${NC} (scanner.c) + input → gera tokens.txt"
echo -e "  3. ${YELLOW}Parser Racket${NC} (racket_parser) + tokens.txt → gera AST"
echo ""
echo -e "${BLUE}Comandos disponíveis:${NC}"
echo -e "  ${YELLOW}./build_and_run.sh${NC}           - Executa fluxo completo (modo normal)"
echo -e "  ${YELLOW}./build_and_run.sh -v${NC}        - Executa fluxo completo (modo verbose)"
echo -e "  ${YELLOW}make all${NC}                     - Compila gerador E parser"
echo -e "  ${YELLOW}make automaton${NC}               - Compila apenas o gerador"
echo -e "  ${YELLOW}make racket${NC}                  - Compila apenas o parser"
echo -e "  ${YELLOW}./scanner <input>${NC}            - Gera tokens.txt do input"
echo -e "  ${YELLOW}./main/racket_parser tokens.txt${NC} - Parser gera AST"
echo -e "  ${YELLOW}./main/racket_parser -l tokens.txt${NC} - Mostra tokens lidos"
echo -e "  ${YELLOW}./main/racket_parser -f${NC}      - Mostra FIRST/FOLLOW sets"
echo ""

# Made with Bob
