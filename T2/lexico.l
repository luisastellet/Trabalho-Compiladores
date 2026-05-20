%{
#include "calc.h"
#include "y.tab.h"
%}

DIGITO      [0-9]
LETRA       [a-zA-Z]
ESPECIAL    [!$%&*+\-./:<=>?@^_~]
IDENTIF     ({LETRA}|{ESPECIAL})({LETRA}|{DIGITO}|{ESPECIAL})*

%%

[ \t\n\r]+          { /* Ignora */ }
";".* { /* Comentário */ }

"("                 { return TOKEN_LPAREN; }
")"                 { return TOKEN_RPAREN; }
"'"                 { return TOKEN_QUOTE; }
"#"                 { return TOKEN_SHARP; }

\"(\\.|[^"\\])*\"   { return TOKEN_STRING; }

("-"?{DIGITO}+("."{DIGITO}*)?) { 
    yylval.valor_num = atof(yytext); 
    return TOKEN_NUM; 
}

{IDENTIF}           {
    symrec *s = getsym(yytext);
    if (s == NULL) s = putsym(yytext, TOKEN_ID);
    yylval.ptr = s;
    return s->type; 
}

.                   { printf("Erro: %s\n", yytext); }

%%

int yywrap() { return 1; }