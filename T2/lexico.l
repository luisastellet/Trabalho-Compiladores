%{
#include "scheme.h"
#include "sintatico.tab.h"

extern int yylineno;  /* Número da linha atual (fornecido pelo Flex) */
%}

DIGITO      [0-9]
LETRA       [a-zA-Z]
ESPECIAL    [!$%&*+\-./:<=>?@^_~]
IDENTIF     ({LETRA}|{ESPECIAL})({LETRA}|{DIGITO}|{ESPECIAL})*

%%

[ \t\r]+          { /* Ignora espaços em branco */ }
"\n"               { yylineno++; }  /* Incrementa contagem de linhas */
";".* { /* Comentário até fim da linha */ }

"("                 { return TOKEN_LPAREN; }
")"                 { return TOKEN_RPAREN; }
"'"                 { return TOKEN_QUOTE_KEYWORD; }

\"(\\.|[^"\\])*\"   { 
    /* Remove as aspas da string */
    char *str = (char *)malloc(strlen(yytext) - 1);
    strncpy(str, yytext + 1, strlen(yytext) - 2);
    str[strlen(yytext) - 2] = '\0';
    yylval.str = str;
    return TOKEN_STRING_LITERAL; 
}

("-"?{DIGITO}+("."{DIGITO}*)?) { 
    yylval.num = atof(yytext); 
    return TOKEN_NUM; 
}

{IDENTIF}           {
    symrec *s = getsym(yytext);
    if (s != NULL) {
        /* Identificador já na tabela; retorna seu tipo */
        yylval.ptr = s;
        return s->type;
    }
    /* Identificador não está na tabela; retorna como TOKEN_ID genérico */
    yylval.ptr = (symrec *)malloc(sizeof(symrec));
    yylval.ptr->name = strdup(yytext);
    yylval.ptr->type = TOKEN_ID;
    return TOKEN_ID;
}

.                   { fprintf(stderr, "Erro léxico: caractere não reconhecido '%s'\n", yytext); }

%%

int yywrap() { return 1; }