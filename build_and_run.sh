#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR"
MAIN_DIR="$SCRIPT_DIR/main"

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}      Gerador de Scanner Léxico - Build e Execução${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

echo -e "${YELLOW}[1/4] Limpando...${NC}"
cd "$BUILD_DIR"
rm -f main/automaton scanner scanner.c
echo -e "${GREEN}OK${NC}\n"

echo -e "${YELLOW}[2/4] Compilando gerador...${NC}"
g++ -std=c++11 -Wall -Wextra -g \
    -o main/automaton \
    main/automaton.cpp \
    syntax/syntaxTree.cpp \
    automata/dfa.cpp \
    automata/nfa.cpp

if [ $? -ne 0 ]; then
    echo -e "${RED}Erro na compilação${NC}"
    exit 1
fi
echo -e "${GREEN}OK${NC}\n"

echo -e "${YELLOW}[3/4] Gerando scanner...${NC}"

if [ ! -f "$BUILD_DIR/input.txt" ]; then
    cp "$MAIN_DIR/input.txt" "$BUILD_DIR/input.txt"
fi

cd "$BUILD_DIR"
./main/automaton

if [ ! -f "$BUILD_DIR/scanner.c" ]; then
    echo -e "${RED}Erro: scanner.c não gerado${NC}"
    exit 1
fi
echo -e "${GREEN}OK${NC}\n"

echo -e "${YELLOW}[4/4] Compilando scanner...${NC}"
gcc -Wall -o scanner scanner.c scanner_main.c

if [ $? -ne 0 ]; then
    echo -e "${RED}Erro na compilação${NC}"
    exit 1
fi
echo -e "${GREEN}OK${NC}\n"

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}      Executando Scanner${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"
./scanner
echo ""
