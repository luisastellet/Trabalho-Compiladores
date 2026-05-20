#include <string.h>
#include <stdlib.h>
#include "scheme.h"
#include "y.tab.h" // Gerado pelo Bison

symrec *sym_table = NULL;

symrec *putsym(char const *name, int type) {
    symrec *ptr = (symrec *) malloc(sizeof(symrec));
    ptr->name = strdup(name);
    ptr->type = type;
    ptr->value.var = 0;
    ptr->next = sym_table;
    sym_table = ptr;
    return ptr;
}

symrec *getsym(char const *name) {
    for (symrec *p = sym_table; p != NULL; p = p->next)
        if (strcmp(p->name, name) == 0) return p;
    return NULL;
}

void inicializar_tabela() {
    
    /* =========================================================================
       1. FORMAS SINTÁTICAS (Keywords que alteram a estrutura gramatical no Bison)
       ========================================================================= */
    
    // Core da Linguagem / Definições e Funções
    putsym("define",          TOKEN_DEFINE); 
    putsym("lambda",          TOKEN_LAMBDA);
    putsym("if",              TOKEN_IF);         
    putsym("let",             TOKEN_LET);
    putsym("let*",            TOKEN_LET_STAR);
    putsym("letrec",          TOKEN_LETREC);
    
    // Condicionais e Fluxo
    putsym("cond",            TOKEN_COND);
    putsym("case",            TOKEN_CASE);
    putsym("and",             TOKEN_AND);
    putsym("or",              TOKEN_OR);
    putsym("else",            TOKEN_ELSE); 
    putsym("=>",              TOKEN_ARROW);  
    
    // Atribuição, Sequenciamento e Blocos
    putsym("begin",           TOKEN_BEGIN);
    putsym("set!",            TOKEN_SET_BANG); 
    putsym("do",              TOKEN_DO);
    putsym("delay",           TOKEN_DELAY);
    
    // Metaprogramação e Citações (Quotes)
    putsym("quote",           TOKEN_QUOTE_KEYWORD);
    putsym("quasiquote",      TOKEN_QUASIQUOTE);
    putsym("unquote",         TOKEN_UNQUOTE);
    putsym("unquote-splicing", TOKEN_UNQUOTE_SPLICING);
    
    // Sistema de Macros R5RS
    putsym("define-syntax",   TOKEN_DEFINE_SYNTAX);
    putsym("let-syntax",      TOKEN_LET_SYNTAX);
    putsym("letrec-syntax",   TOKEN_LETREC_SYNTAX);
    putsym("syntax-rules",    TOKEN_SYNTAX_RULES);


    /* =========================================================================
       2. PROCEDIMENTOS PRIMITIVOS (Funções nativas R5RS - Todas nascem como TOKEN_ID)
       ========================================================================= */
    char *primitivas[] = {
        // Aritmética e Comparações
        "+", "-", "*", "/", "=", "<", ">", "<=", ">=", "abs", "max", "min",
        "quotient", "remainder", "modulo", "gcd", "lcm", "exp", "log", "sin",
        "cos", "tan", "sqrt", "expt", "floor", "ceiling", "truncate", "round",
        
        // Verificação de Tipos (Predicados)
        "number?", "complex?", "real?", "rational?", "integer?", "boolean?",
        "symbol?", "char?", "string?", "vector?", "procedure?", "pair?", "list?",
        "null?", "eof-object?", "input-port?", "output-port?",
        
        // Estruturas de Dados (Listas)
        "cons", "car", "cdr", "set-car!", "set-cdr!", "caar", "cadr", "cdar", "cddr",
        "list", "length", "append", "reverse", "list-tail", "list-ref", "memq", "memv",
        "member", "assq", "assv", "assoc",
        
        // Manipulação de Texto
        "char=?", "char<?", "char>?", "char<=?", "char>=?", "string=?", "string-length",
        "string-append", "string->list", "list->string", "symbol->string", "string->symbol",
        
        // Sistema e Entrada/Saída
        "eq?", "eqv?", "equal?", "not", "display", "newline", "write", "read",
        "open-input-file", "open-output-file", "close-input-port", "close-output-port",
        "apply", "map", "for-each", "eval", "force", "call-with-current-continuation", 
        NULL // O ponteiro NULL no final garante que o loop saiba onde parar
    };

    // Este loop varre o array acima e joga tudo na mesma tabela como TOKEN_ID
    for (int i = 0; primitivas[i] != NULL; i++) {
         putsym(primitivas[i], TOKEN_ID);
    }
}