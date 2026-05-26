CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

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
all: $(TARGET)

# Compilação do Bison (gera sintatico.tab.c e sintatico.tab.h)
sintatico.tab.c sintatico.tab.h: $(BISON_FILE)
	bison -d $(BISON_FILE)

# Compilação do Flex (depende de sintatico.tab.h)
lex.yy.c: $(FLEX_FILE) sintatico.tab.h
	flex -o lex.yy.c $(FLEX_FILE)

# Compilação de objetos do Flex
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c -o $@

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

.PHONY: all test clean
