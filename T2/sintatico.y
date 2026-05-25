%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheme.h"

int yylex(void);
void yyerror(char const *s);
FILE *arquivo_python;

extern int yylineno;  /* Número da linha atual (fornecido pelo Flex) */

/* Declarações de funções para type checking */
extern int check_types(ast_node *node);
extern void clear_type_errors(void);
extern int has_type_errors(void);

/* Constantes de Tipo para Verificação Semântica */
#define T_NUM 1
#define T_STR 2
#define T_BOL 3
#define T_LST 4
#define T_ANY 0
%}

%union {
    ast_node *node;         /* Nó da AST */
    ast_node_list *args;    /* Lista de nós AST */
    char *str;              /* Para strings literal no lexer */
    double num;             /* Para números */
    symrec *ptr;            /* Para símbolos da tabela */
}

/* Tokens de Valores */
%token <num> TOKEN_NUM
%token <str> TOKEN_STRING_LITERAL
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
%type <node> expressao comando_lista cond_clause binding
%type <args> lista_argumentos binding_list cond_clauses

/* Destrutor: libera AST quando descartado */
%destructor { free_ast($$); } expressao comando_lista cond_clause binding
%destructor { free_arg_list($$); } lista_argumentos binding_list cond_clauses

%%

/* Ponto de entrada: vai acumulando o código Python */
programa:
    expressao { 
        /* Verificar tipos antes de gerar código */
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
        /* Verificar tipos antes de gerar código */
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

/* Expressões básicas e compostas */
expressao:
    TOKEN_NUM {
        $$ = create_num($1);
    }
    | TOKEN_STRING_LITERAL {
        $$ = create_str($1);
        free($1);
    }
    | TOKEN_ID {
        /* VERIFICAÇÃO DE CONTEXTO */
        symrec *s = getsym($1->name);
        if (s == NULL) {
            /* Se não existe, cria uma entrada (pode ser parâmetro de lambda/let) */
            s = putsym($1->name, TOKEN_ID);
            /* Marca como não declarado inicialmente; será marcado como declarado
               se for parâmetro de lambda/let */
        }
        marcar_como_usado($1->name);  /* Marca como usado */
        $$ = create_id($1->name);
    }
    | TOKEN_LPAREN comando_lista TOKEN_RPAREN {
        $$ = $2;
    }
    ;

/* Comandos e formas especiais (descem de ( ) ) */
comando_lista:
    /* --- DEFINIÇÕES --- */
    TOKEN_DEFINE TOKEN_ID expressao {
        symrec *s = putsym($2->name, TOKEN_ID); // Registra no contexto
        s->foi_declarado = 1;  /* Marca como declarado */
        $$ = create_define($2->name, $3);
    }

    /* --- CONDICIONAIS --- */
    | TOKEN_IF expressao expressao expressao {
        $$ = create_if($2, $3, $4);
    }
    
    /* --- AND/OR (operadores lógicos) --- */
    | TOKEN_AND lista_argumentos {
        /* (and a b c) -> (a and b and c) em Python */
        char *codigo = codegen_args($2, " and ");
        $$ = create_id(codigo); /* Simplificado: retorna como ID */
        free(codigo);
        free_arg_list($2);
    }
    
    | TOKEN_OR lista_argumentos {
        /* (or a b c) -> (a or b or c) em Python */
        char *codigo = codegen_args($2, " or ");
        $$ = create_id(codigo); /* Simplificado: retorna como ID */
        free(codigo);
        free_arg_list($2);
    }

    /* --- BLOCOS E SEQUENCIAMENTO --- */
    | TOKEN_BEGIN lista_argumentos {
        $$ = create_begin($2);
    }

    /* --- ATRIBUIÇÃO --- */
    | TOKEN_SET_BANG TOKEN_ID expressao {
        $$ = create_set($2->name, $3);
    }

    /* --- CITAÇÃO --- */
    | TOKEN_QUOTE_KEYWORD expressao {
        $$ = create_quote($2);
    }

    /* --- REGRA GENÉRICA: Operadores e Chamadas de Função --- */
    | TOKEN_ID lista_argumentos {
        /* Aqui diferenciamos operadores especiais de chamadas de função */
        
        /* Garante que o identificador existe na tabela de símbolos */
        symrec *s = getsym($1->name);
        if (s == NULL) {
            s = putsym($1->name, TOKEN_ID);
        }
        
        marcar_como_usado($1->name);  /* Marca identificador como usado */
        
        /* 1. OPERADORES ARITMÉTICOS INFIXOS */
        if (strcmp($1->name, "+") == 0 || strcmp($1->name, "-") == 0 || 
            strcmp($1->name, "*") == 0 || strcmp($1->name, "/") == 0) {
            
            /* Converte LSE de argumentos para binop aninhado (esquerda-associativo) */
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
        /* 2. OPERADORES COMPARAÇÃO INFIXOS */
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
        /* 3. OPERADOR DISPLAY (traduz para print) */
        else if (strcmp($1->name, "display") == 0) {
            if ($2 == NULL) {
                yyerror("display requer um argumento");
                YYERROR;
            }
            
            /* Cria nó de chamada com nome "print" */
            $$ = create_call("print", $2);
        }
        /* 4. PRIMITIVAS GENÉRICAS E FUNÇÕES DO USUÁRIO */
        else {
            $$ = create_call($1->name, $2);
        }
    }
    
    /* --- LAMBDA --- */
    | TOKEN_LAMBDA TOKEN_LPAREN lista_argumentos TOKEN_RPAREN expressao {
        /* (lambda (x y z) corpo) */
        /* Marca parâmetros como declarados */
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
    
    /* --- LET --- */
    | TOKEN_LET TOKEN_LPAREN binding_list TOKEN_RPAREN expressao {
        /* (let ((x 1) (y 2)) corpo) */
        /* Marca variáveis de binding como declaradas */
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
    
    /* --- LET* --- */
    | TOKEN_LET_STAR TOKEN_LPAREN binding_list TOKEN_RPAREN expressao {
        /* (let* ((x 1) (y 2)) corpo) - similar ao let por enquanto */
        /* Marca variáveis de binding como declaradas */
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
    
    /* --- LETREC --- */
    | TOKEN_LETREC TOKEN_LPAREN binding_list TOKEN_RPAREN expressao {
        /* (letrec ((x 1) (y 2)) corpo) - similar ao let por enquanto */
        /* Marca variáveis de binding como declaradas */
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
    
    /* --- COND --- */
    | TOKEN_COND cond_clauses {
        /* (cond (teste1 expr1) (teste2 expr2) (else expr3)) */
        $$ = create_cond($2);
    }
    
    /* --- CASE --- */
    | TOKEN_CASE expressao cond_clauses {
        /* (case expr (valor1 expr1) (valor2 expr2) ...) */
        /* TODO: Implementar pattern matching baseado em $2 */
        (void)$2; /* Silencia warning de variável não usada */
        $$ = create_cond($3);
    }
    
    ;

/* Binding para let: (nome valor) */
binding:
    TOKEN_LPAREN TOKEN_ID expressao TOKEN_RPAREN {
        /* Cria um "pseudo-nó" que representa a binding
           Usando NODE_LET como container (não é o melhor, mas funciona)
           node->data.define.variavel = nome
           node->data.define.valor = expressão
        */
        $$ = create_define($2->name, $3);
    }
    ;

/* Lista de bindings */
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

/* Cláusula de cond: (teste resultado) ou (else resultado) */
cond_clause:
    TOKEN_LPAREN expressao expressao TOKEN_RPAREN {
        /* (teste resultado) */
        $$ = create_cond_clause($2, $3);
    }
    | TOKEN_LPAREN TOKEN_ELSE expressao TOKEN_RPAREN {
        /* (else resultado) - cria teste com valor "else" */
        ast_node *teste = create_id("else");
        $$ = create_cond_clause(teste, $3);
    }
    ;

/* Lista de argumentos (LSE de nós AST) */
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

/* Lista de cláusulas de cond */
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

    printf("Convertendo Scheme para Python...\n");
    int result = yyparse();
    
    if (result == 0) {
        printf("Arquivo 'saida.py' gerado com sucesso!\n");
        
        /* Verifica se variáveis foram usadas mas não foram declaradas com define */
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
            verificar_nao_utilizados();  /* Verifica e reporta variáveis/funções não usadas */
        }
    } else {
        printf("Erro na compilação\n");
    }
    
    fclose(arquivo_python);
    return result;
}