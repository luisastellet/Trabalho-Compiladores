CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
FLEX_CFLAGS = $(CFLAGS) -Wno-unused-function -Wno-unneeded-internal-declaration

# Arquivos-fonte
FLEX_FILE = lexico.l
BISON_FILE = sintatico.y
C_SOURCES = tabela_simbolos.c ast.c

# Arquivos gerados
FLEX_OUT = lex.yy.c
BISON_OUT = sintatico.tab.c
BISON_HEADER = sintatico.tab.h

# Objeto e executável
OBJECTS = lex.yy.o sintatico.tab.o tabela_simbolos.o ast.o
TARGET = transpilador

# Regra padrão
all: check-deps $(TARGET)

# ============================================================================
# VERIFICAÇÃO DE DEPENDÊNCIAS
# ============================================================================

.PHONY: check-deps

check-deps:
	@echo " Verificando dependências necessárias...                            "
	@echo ""
	
	@# Verificar gcc
	@if ! command -v gcc &> /dev/null; then \
		echo "gcc não encontrado! Instale com:"; \
		echo "   macOS: brew install gcc"; \
		echo "   Linux: sudo apt-get install build-essential"; \
		exit 1; \
	fi
	@echo " gcc instalado"
	
	@# Verificar flex
	@if ! command -v flex &> /dev/null; then \
		echo "flex não encontrado! Instale com:"; \
		echo "   macOS: brew install flex"; \
		echo "   Linux: sudo apt-get install flex"; \
		exit 1; \
	fi
	@echo " flex instalado"
	
	@# Verificar bison
	@if ! command -v bison &> /dev/null; then \
		echo "bison não encontrado! Instale com:"; \
		echo "   macOS: brew install bison"; \
		echo "   Linux: sudo apt-get install bison"; \
		exit 1; \
	fi
	@echo " bison instalado"
	
	@# Verificar python3
	@if ! command -v python3 &> /dev/null; then \
		echo "python3 não encontrado! Instale com:"; \
		echo "   macOS: brew install python@3.11"; \
		echo "   Linux: sudo apt-get install python3"; \
		exit 1; \
	fi
	@echo " python3 instalado"
	
	@# Verificar racket (apenas aviso, não é crítico para compilação)
	@if ! command -v racket &> /dev/null; then \
		echo "  racket não encontrado (necessário para testes completos)"; \
		echo "   Instale com:"; \
		echo "   macOS: brew install racket"; \
		echo "   Linux: sudo apt-get install racket"; \
		echo ""; \
	else \
		echo " racket instalado"; \
	fi
	
	@echo ""
	@echo " Todas as dependências críticas estão instaladas!"
	@echo ""

# Compilação do Bison (gera sintatico.tab.c e sintatico.tab.h)
sintatico.tab.c sintatico.tab.h: $(BISON_FILE)
	bison -d $(BISON_FILE)

# Compilação do Flex (depende de sintatico.tab.h)
lex.yy.c: $(FLEX_FILE) sintatico.tab.h
	flex -o lex.yy.c $(FLEX_FILE)

# Compilação de objetos do Flex
lex.yy.o: lex.yy.c
	$(CC) $(FLEX_CFLAGS) -c lex.yy.c -o $@

# Compilação de objetos do Bison
sintatico.tab.o: sintatico.tab.c
	$(CC) $(CFLAGS) -c sintatico.tab.c -o $@

# Compilação de objetos C
tabela_simbolos.o: tabela_simbolos.c scheme.h
	$(CC) $(CFLAGS) -c tabela_simbolos.c -o $@

ast.o: ast.c scheme.h
	$(CC) $(CFLAGS) -c ast.c -o $@

# Link final
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm

# Limpeza
clean:
	rm -f $(FLEX_OUT) $(BISON_OUT) $(BISON_HEADER)
	rm -f *.o $(TARGET) saida.py

.PHONY: all test clean check-deps
