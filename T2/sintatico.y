%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheme.h"

int yylex(void);
void yyerror(char const *s);
FILE *arquivo_python;

extern int yylineno;  

extern int check_types(ast_node *node);
extern void clear_type_errors(void);
extern int has_type_errors(void);

#define T_NUM 1
#define T_STR 2
#define T_BOL 3
#define T_LST 4
#define T_ANY 0
%}

%union {
    ast_node *node;         
    ast_node_list *args;    
    char *str;              
    double num;             
    symrec *ptr;            
}

%token <num> TOKEN_NUM
%token <str> TOKEN_STRING_LITERAL
%token <ptr> TOKEN_ID

%token TOKEN_DEFINE TOKEN_LAMBDA TOKEN_IF TOKEN_LET TOKEN_LET_STAR TOKEN_LETREC
%token TOKEN_COND TOKEN_CASE TOKEN_AND TOKEN_OR TOKEN_ELSE
%token TOKEN_BEGIN TOKEN_SET_BANG
%token TOKEN_QUOTE_KEYWORD

%token TOKEN_LPAREN TOKEN_RPAREN

%type <node> expressao comando_lista cond_clause binding
%type <args> lista_argumentos binding_list cond_clauses

%destructor { free_ast($$); } expressao comando_lista cond_clause binding
%destructor { free_arg_list($$); } lista_argumentos binding_list cond_clauses

%%

programa:
    expressao { 
        clear_type_errors();
        if (!check_types($1)) {
            fprintf(stderr, "\nCOMPILACAO ABORTADA: Erros de tipo detectados!\n\n");
            YYERROR;
        }
        
        char *codigo = codegen($1);
        fprintf(arquivo_python, "%s\n", codigo);
        free(codigo);
    }
    | programa expressao { 
        clear_type_errors();
        if (!check_types($2)) {
            fprintf(stderr, "\nCOMPILACAO ABORTADA: Erros de tipo detectados!\n\n");
            YYERROR;
        }
        
        char *codigo = codegen($2);
        fprintf(arquivo_python, "%s\n", codigo);
        free(codigo);
    }
    ;

expressao:
    TOKEN_NUM {
        $$ = create_num($1);
    }
    | TOKEN_STRING_LITERAL {
        $$ = create_str($1);
        free($1);
    }
    | TOKEN_ID {
        symrec *s = getsym($1->name);
        if (s == NULL) {
            s = putsym($1->name, TOKEN_ID);
        }
        $$ = create_id($1->name);
    }
    | TOKEN_LPAREN comando_lista TOKEN_RPAREN {
        $$ = $2;
    }
    ;

comando_lista:
    TOKEN_DEFINE TOKEN_ID expressao {
        symrec *s = putsym($2->name, TOKEN_ID);
        s->foi_declarado = 1;
        $$ = create_define($2->name, $3);
    }

    | TOKEN_IF expressao expressao expressao {
        $$ = create_if($2, $3, $4);
    }
    
    | TOKEN_AND lista_argumentos {
        char *codigo = codegen_args($2, " and ");
        $$ = create_id(codigo);
        free(codigo);
        free_arg_list($2);
    }
    
    | TOKEN_OR lista_argumentos {
        char *codigo = codegen_args($2, " or ");
        $$ = create_id(codigo);
        free(codigo);
        free_arg_list($2);
    }

    | TOKEN_BEGIN lista_argumentos {
        $$ = create_begin($2);
    }

    | TOKEN_SET_BANG TOKEN_ID expressao {
        $$ = create_set($2->name, $3);
    }

    | TOKEN_QUOTE_KEYWORD expressao {
        $$ = create_quote($2);
    }

    | TOKEN_ID lista_argumentos {
        symrec *s = getsym($1->name);
        if (s == NULL) {
            s = putsym($1->name, TOKEN_ID);
        }
        
        marcar_como_usado($1->name);
        s->foi_usado_como = 2;
        
        if (strcmp($1->name, "+") == 0 || strcmp($1->name, "-") == 0 || 
            strcmp($1->name, "*") == 0 || strcmp($1->name, "/") == 0) {
            
            if ($2 == NULL) {
                yyerror("Operador matemático requer argumentos");
                YYERROR;
            }
            
            ast_node *result = $2->node;
            ast_node_list *current = $2->next;
            
            while (current != NULL) {
                result = create_binop($1->name, result, current->node);
                current = current->next;
            }
            
            free_arg_list($2);
            $$ = result;
        }
        else if (strcmp($1->name, ">") == 0 || strcmp($1->name, "<") == 0 || 
                 strcmp($1->name, ">=") == 0 || strcmp($1->name, "<=") == 0 ||
                 strcmp($1->name, "=") == 0) {
            
            if ($2 == NULL || $2->next == NULL) {
                yyerror("Operador de comparação requer exatamente 2 argumentos");
                YYERROR;
            }
            
            $$ = create_binop($1->name, $2->node, $2->next->node);
            free_arg_list($2);
        }
        else if (strcmp($1->name, "display") == 0) {
            if ($2 == NULL) {
                yyerror("display requer um argumento");
                YYERROR;
            }
            
            $$ = create_call("print", $2);
        }
        else {
            $$ = create_call($1->name, $2);
        }
    }
    
    | TOKEN_LAMBDA TOKEN_LPAREN lista_argumentos TOKEN_RPAREN expressao {
        ast_node_list *param = $3;
        while (param != NULL) {
            if (param->node->type == NODE_ID) {
                symrec *s = getsym(param->node->data.id.nome);
                if (s != NULL) {
                    s->foi_declarado = 1;
                }
            }
            param = param->next;
        }
        $$ = create_lambda($3, $5);
    }
    
    | TOKEN_LET TOKEN_LPAREN binding_list TOKEN_RPAREN expressao {
        ast_node_list *binding = $3;
        while (binding != NULL) {
            if (binding->node->type == NODE_DEFINE) {
                symrec *s = getsym(binding->node->data.define.variavel);
                if (s != NULL) {
                    s->foi_declarado = 1;
                }
            }
            binding = binding->next;
        }
        $$ = create_let($3, $5);
    }
    
    | TOKEN_LET_STAR TOKEN_LPAREN binding_list TOKEN_RPAREN expressao {
        ast_node_list *binding = $3;
        while (binding != NULL) {
            if (binding->node->type == NODE_DEFINE) {
                symrec *s = getsym(binding->node->data.define.variavel);
                if (s != NULL) {
                    s->foi_declarado = 1;
                }
            }
            binding = binding->next;
        }
        $$ = create_let($3, $5);
    }
    
    | TOKEN_LETREC TOKEN_LPAREN binding_list TOKEN_RPAREN expressao {
        ast_node_list *binding = $3;
        while (binding != NULL) {
            if (binding->node->type == NODE_DEFINE) {
                symrec *s = getsym(binding->node->data.define.variavel);
                if (s != NULL) {
                    s->foi_declarado = 1;
                }
            }
            binding = binding->next;
        }
        $$ = create_let($3, $5);
    }
    
    | TOKEN_COND cond_clauses {
        $$ = create_cond($2);
    }
    
    | TOKEN_CASE expressao cond_clauses {
        (void)$2; 
        $$ = create_cond($3);
    }
    
    ;

binding:
    TOKEN_LPAREN TOKEN_ID expressao TOKEN_RPAREN {
        $$ = create_define($2->name, $3);
    }
    ;

binding_list:
    {
        $$ = NULL;
    }
    | binding {
        $$ = create_arg_node($1);
    }
    | binding_list binding {
        $$ = append_arg_node($1, $2);
    }
    ;

cond_clause:
    TOKEN_LPAREN expressao expressao TOKEN_RPAREN {
        $$ = create_cond_clause($2, $3);
    }
    | TOKEN_LPAREN TOKEN_ELSE expressao TOKEN_RPAREN {
        ast_node *teste = create_id("else");
        $$ = create_cond_clause(teste, $3);
    }
    ;

lista_argumentos:
    {
        $$ = NULL;
    }
    | expressao {
        $$ = create_arg_node($1);
    }
    | lista_argumentos expressao {
        $$ = append_arg_node($1, $2);
    }
    ;

cond_clauses:
    {
        $$ = NULL;
    }
    | cond_clause {
        $$ = create_arg_node($1);
    }
    | cond_clauses cond_clause {
        $$ = append_arg_node($1, $2);
    }
    ;

%%

void yyerror(char const *s) {
    fprintf(stderr, "[ERRO NO COMPILADOR]: %s\n", s);
}

int main() {
    inicializar_tabela();
    arquivo_python = fopen("saida.py", "w");
    
    if (!arquivo_python) {
        fprintf(stderr, "Erro: não foi possível abrir arquivo saida.py\n");
        return 1;
    }

    int result = yyparse();
    
    if (result == 0) {

        int erros = 0;
        for (symrec *p = sym_table; p != NULL; p = p->next) {
            if (!p->eh_builtin && p->foi_usado && !p->foi_declarado && p->type == TOKEN_ID) {
                relatorio_erro_nao_declarado(p->name, p->linha_declaracao);
                erros++;
            }
        }
        
        if (erros > 0) {
            result = 1;  /* Marca como erro para não gerar saída */
            printf("Erro na compilacao\n");
        } else {
            verificar_nao_utilizados();  
            printf("Arquivo 'saida.py' gerado com sucesso!\n");
        }
    } else {
        printf("Erro na compilação\n");
    }
    
    fclose(arquivo_python);
    return result;
}