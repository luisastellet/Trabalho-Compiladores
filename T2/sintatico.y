%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheme.h"

int yylex(void);
void yyerror(char const *s);
FILE *arquivo_python;

/* Constantes de Tipo para Verificação Semântica */
#define T_NUM 1
#define T_STR 2
#define T_BOL 3
#define T_LST 4
#define T_ANY 0
%}

%union {
    double valor_num;
    char *valor_str;
    struct {
        char *texto;       
        int tipo;          
    } nodo;
    symrec *ptr;
}

/* Tokens de Valores */
%token <valor_num> TOKEN_NUM
%token <valor_str> TOKEN_STRING_LITERAL
%token <ptr> TOKEN_ID

/* --- TOKENS QUE VOCÊ DEFINIU NA TABELA --- */
%token TOKEN_DEFINE TOKEN_LAMBDA TOKEN_IF TOKEN_LET TOKEN_LET_STAR TOKEN_LETREC
%token TOKEN_COND TOKEN_CASE TOKEN_AND TOKEN_OR TOKEN_ELSE TOKEN_ARROW
%token TOKEN_BEGIN TOKEN_SET_BANG TOKEN_DO TOKEN_DELAY
%token TOKEN_QUOTE_KEYWORD TOKEN_QUASIQUOTE TOKEN_UNQUOTE TOKEN_UNQUOTE_SPLICING
%token TOKEN_DEFINE_SYNTAX TOKEN_LET_SYNTAX TOKEN_LETREC_SYNTAX TOKEN_SYNTAX_RULES

/* Sinais Gráficos */
%token TOKEN_LPAREN TOKEN_RPAREN

/* Definição de Tipagem das Regras */
%type <nodo> expressao lista_argumentos comando_lista programa

/* Sempre que um token ou regra que usa o campo 'texto' for descartado, 
   o Bison chama o free automaticamente. */
%destructor { free($$.texto); } expressao comando_lista lista_argumentos

%%

/* Ponto de entrada: vai acumulando o código Python */
programa:
    expressao { 
        fprintf(arquivo_python, "%s\n", $1.texto); 
        free($1.texto);
    }
    | programa expressao { 
        fprintf(arquivo_python, "%s\n", $2.texto); 
        free($2.texto);
    }
    ;

expressao:
    TOKEN_NUM {
        $$.tipo = T_NUM;
        $$.texto = malloc(32);
        sprintf($$.texto, "%g", $1);
    }
    | TOKEN_STRING_LITERAL {
        $$.tipo = T_STR;
        $$.texto = strdup($1);
    }
    | TOKEN_ID {
        /* VERIFICAÇÃO DE CONTEXTO */
        symrec *s = getsym($1->name);
        if (s == NULL) {
            char msg[128];
            sprintf(msg, "Erro de Contexto: O identificador '%s' nao existe!", $1->name);
            yyerror(msg);
            YYERROR;
        }
        $$.tipo = T_ANY;
        $$.texto = strdup($1->name);
    }
    | TOKEN_LPAREN comando_lista TOKEN_RPAREN {
        $$ = $2;
    }
    ;

comando_lista:
    /* --- DEFINIÇÕES --- */
    TOKEN_DEFINE TOKEN_ID expressao {
        $$.texto = malloc(strlen($2->name) + strlen($3.texto) + 5);
        sprintf($$.texto, "%s = %s", $2->name, $3.texto);
        putsym($2->name, TOKEN_ID); // Registra no contexto
    }

    /* --- CONDICIONAIS --- */
    | TOKEN_IF expressao expressao expressao {
        $$.texto = malloc(strlen($2.texto) + strlen($3.texto) + strlen($4.texto) + 25);
        sprintf($$.texto, "(%s if %s else %s)", $3.texto, $2.texto, $4.texto);
    }
    | TOKEN_AND lista_argumentos {
        // (and a b) -> (a and b) em Python
        $$.texto = strdup($2.texto); // Aqui precisaria de um replace de ',' por 'and'
    }

    /* --- BLOCOS E SEQUENCIAMENTO --- */
    | TOKEN_BEGIN lista_argumentos {
        // begin não tem equivalente direto em expressão de uma linha, 
        // geralmente requer lógica de blocos
        $$.texto = strdup($2.texto);
    }

    /* --- ATRIBUIÇÃO --- */
    | TOKEN_SET_BANG TOKEN_ID expressao {
        $$.texto = malloc(strlen($2->name) + strlen($3.texto) + 5);
        sprintf($$.texto, "%s = %s", $2->name, $3.texto);
    }

    /* --- REGRA GENÉRICA: Operadores e Chamadas de Função --- */
    | TOKEN_ID lista_argumentos {
        // 1. TRATAMENTO PARA OPERADORES ARITMÉTICOS (Infixos)
        if (strcmp($1->name, "+") == 0 || strcmp($1->name, "-") == 0 || 
            strcmp($1->name, "*") == 0 || strcmp($1->name, "/") == 0) {
            
            // VERIFICAÇÃO DE TIPO: O operador matemático exige números
            // Nota: Em um sistema real, você checaria se o tipo da lista_argumentos é T_NUM
            if ($2.tipo != T_NUM && $2.tipo != T_ANY) {
                yyerror("Erro de Tipo: Operadores aritmeticos exigem argumentos numericos.");
                free($2.texto);
                YYERROR;
            }

            // Alocamos espaço para o texto. 
            // Vamos trocar as vírgulas da lista pelo operador.
            $$.texto = strdup($2.texto);
            
            // Lógica simples: percorre a string e troca ',' pelo operador
            for (int i = 0; $$.texto[i]; i++) {
                if ($$.texto[i] == ',') $$.texto[i] = $1->name[0];
            }
            
            // Envolvemos em parênteses para garantir a precedência (ex: (1 + 2 + 3))
            char *temp = malloc(strlen($$.texto) + 3);
            sprintf(temp, "(%s)", $$.texto);
            free($$.texto);
            $$.texto = temp;
            $$.tipo = T_NUM;
        } 
        
        // 2. TRATAMENTO PARA DISPLAY (Tradução de nome)
        else if (strcmp($1->name, "display") == 0) {
            $$.texto = malloc(strlen($2.texto) + 10);
            sprintf($$.texto, "print(%s)", $2.texto);
            $$.tipo = T_ANY;
        }

        // 3. PRIMITIVAS GENÉRICAS E FUNÇÕES DO USUÁRIO (Prefixas)
        else {
            // Ex: (abacate 1 2) -> abacate(1, 2)
            $$.texto = malloc(strlen($1->name) + strlen($2.texto) + 5);
            sprintf($$.texto, "%s(%s)", $1->name, $2.texto);
            $$.tipo = T_ANY; // O retorno depende da definição da função
        }

        // LIMPEZA: Como já usamos o texto da lista_argumentos, liberamos a memória
        free($2.texto);
    }
    
    /* DICA: Você deve adicionar aqui os cases para TOKEN_LET, TOKEN_LAMBDA, etc.
       Seguindo o mesmo padrão de malloc/sprintf para gerar o código Python.
    */
    ;

lista_argumentos:
    expressao {
        $$.texto = strdup($1.texto);
    }
    | lista_argumentos expressao {
        $$.texto = malloc(strlen($1.texto) + strlen($2.texto) + 3);
        sprintf($$.texto, "%s, %s", $1.texto, $2.texto);
    }
    ;

%%

void yyerror(char const *s) {
    fprintf(stderr, "[ERRO NO COMPILADOR]: %s\n", s);
}

int main() {
    inicializar_tabela_r5rs();
    arquivo_python = fopen("saida.py", "w");
    
    if (!arquivo_python) return 1;

    printf("Convertendo Scheme para Python...\n");
    if (yyparse() == 0) {
        printf("Arquivo 'saida.py' gerado com sucesso!\n");
    }
    
    fclose(arquivo_python);
    return 0;
}