#include <string.h>
#include <stdlib.h>
#include "scheme.h"
#include "sintatico.tab.h" 

extern int yylineno;  

symrec *sym_table = NULL;

symrec *putsym(char const *name, int type) {
    extern int yylineno;
    symrec *ptr = (symrec *) malloc(sizeof(symrec));
    ptr->name = strdup(name);
    ptr->type = type;
    ptr->value.var = 0;
    ptr->linha_declaracao = yylineno;
    ptr->foi_usado = 0;
    ptr->foi_usado_como = 0;
    ptr->eh_builtin = 0;  
    ptr->foi_declarado = 0;  
    ptr->next = sym_table;
    sym_table = ptr;
    return ptr;
}

symrec *getsym(char const *name) {
    for (symrec *p = sym_table; p != NULL; p = p->next)
        if (strcmp(p->name, name) == 0) return p;
    return NULL;
}

void marcar_como_usado(const char *name) {
    symrec *s = getsym(name);
    if (s != NULL) {
        s->foi_usado = 1;
    }
}

void verificar_nao_utilizados(void) {
    fprintf(stderr, "\n");
    int alertas = 0;
    
    for (symrec *p = sym_table; p != NULL; p = p->next) {
        if (!p->eh_builtin && !p->foi_usado && p->type == TOKEN_ID && p->linha_declaracao > 0) {
            fprintf(stderr, "AVISO: Variavel/funcao '%s' declarada na linha %d mas nunca usada\n",
                    p->name, p->linha_declaracao);
            alertas++;
        }
    }
    
    if (alertas > 0) {
        fprintf(stderr, "\n%d aviso(s) de variavel/funcao nao utilizada\n", alertas);
    }
}

void relatorio_erro_nao_declarado(const char *name, int linha) {
    symrec *s = getsym(name);
    if (s && s->foi_usado_como == 2) {
        fprintf(stderr, "ERRO: Funcao '%s' nao foi declarada (usada na linha %d)\n", name, linha);
    } else {
        fprintf(stderr, "ERRO: Variavel '%s' nao foi declarada (usada na linha %d)\n", name, linha);
    }
}


void inicializar_tabela() {
 
    putsym("define",          TOKEN_DEFINE); 
    putsym("lambda",          TOKEN_LAMBDA);
    putsym("if",              TOKEN_IF);         
    putsym("let",             TOKEN_LET);
    putsym("let*",            TOKEN_LET_STAR);
    putsym("letrec",          TOKEN_LETREC);
    
    putsym("cond",            TOKEN_COND);
    putsym("case",            TOKEN_CASE);
    putsym("and",             TOKEN_AND);
    putsym("or",              TOKEN_OR);
    putsym("else",            TOKEN_ELSE); 
        
    putsym("begin",           TOKEN_BEGIN);
    putsym("set!",            TOKEN_SET_BANG); 
    
    putsym("quote",           TOKEN_QUOTE_KEYWORD);


    char *primitivas[] = {
        "+", "-", "*", "/", "=", "<", ">", "<=", ">=", "abs", "max", "min",
        "quotient", "remainder", "modulo", "gcd", "lcm", "exp", "log", "sin",
        "cos", "tan", "sqrt", "expt", "floor", "ceiling", "truncate", "round",
        
        "number?", "complex?", "real?", "rational?", "integer?", "boolean?",
        "symbol?", "char?", "string?", "vector?", "procedure?", "pair?", "list?",
        "null?", "eof-object?", "input-port?", "output-port?",
        
        "cons", "car", "cdr", "set-car!", "set-cdr!", "caar", "cadr", "cdar", "cddr",
        "list", "length", "append", "reverse", "list-tail", "list-ref", "memq", "memv",
        "member", "assq", "assv", "assoc",
        
        "char=?", "char<?", "char>?", "char<=?", "char>=?", "string=?", "string-length",
        "string-append", "string->list", "list->string", "symbol->string", "string->symbol",
        
        "eq?", "eqv?", "equal?", "not", "display", "newline", "write", "read",
        "open-input-file", "open-output-file", "close-input-port", "close-output-port",
        "apply", "map", "for-each", "eval", "force", "call-with-current-continuation", 
        NULL 
    };

    for (int i = 0; primitivas[i] != NULL; i++) {
         symrec *s = putsym(primitivas[i], TOKEN_ID);
         s->eh_builtin = 1; 
    }
}