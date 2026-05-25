#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
   PARTE 1: TABELA DE SÍMBOLOS (Mantém compatibilidade com código existente)
   ============================================================================ */

union content{                                  /* Union serve como um OU, ou seja, o conteúdo é valor de variável OU uma função */
    double var;                                 /* Para armazenar valores de variáveis */
    double (*fnptr)(double);                    /* Para armazenar ponteiros de funções */ 
};

typedef struct symrec {                         /* Estrutura do Symbol Record*/
    char *name;                                 /* Nome do símbolo (ex: "x", "+", "define")*/
    int type;                                   /* Tipo do símbolo guardado (ex: "TOKEN_ID", "TOKEN_DEFINE")*/
    union content value;                        /* Valor do conteúdo, pela lógica do union pode ser de fato uma variável, ou uma função */
    int linha_declaracao;                       /* Linha onde foi declarado */
    int foi_usado;                              /* 1 se foi usado, 0 se não */
    int foi_usado_como;                         /* 0=não, 1=variável, 2=função */
    int eh_builtin;                             /* 1 se é built-in do Scheme, 0 se é definido pelo usuário */
    int foi_declarado;                          /* 1 se foi declarado com define ou é parâmetro, 0 se apenas criado por referência */
    struct symrec *next;                        /* Ponteiro para o próximo Símbolo, já que é uma LSE */
} symrec;

extern symrec *sym_table;                       /* Qualquer arquivo que tenha um "include scheme.h" podem acessar e modificar a sym_table*/

symrec *putsym(char const *name, int type);     /* Função para inserir um novo símbolo na tabela */
symrec *getsym(char const *name);               /* Função para buscar se um símbolo existe */
void inicializar_tabela();                      /* Função para inicializar a tabela de símbolos */
void marcar_como_usado(const char *name);       /* Marca símbolo como usado */
void verificar_nao_utilizados(void);            /* Verifica e reporta variáveis/funções não utilizadas */
void relatorio_erro_nao_declarado(const char *name, int linha);  /* Reporta erro de variável não declarada */
/* ============================================================================
   PARTE 2: TIPOS DE VALOR (para verificação semântica)
   ============================================================================ */

/* Tipos de valores em Scheme */
typedef enum {
    T_NUM,      /* Número */
    T_STR,      /* String */
    T_BOOL,     /* Booleano */
    T_FUNC,     /* Função/Lambda */
    T_ANY,      /* Tipo desconhecido/qualquer */
    T_ERROR     /* Tipo de erro */
} value_type;

/* ============================================================================
   PARTE 3: ABSTRACT SYNTAX TREE (AST)
   ============================================================================ */

/* Tipos de nó da AST */
typedef enum {
    NODE_NUM,           /* Número literal (ex: 42) */
    NODE_STR,           /* String literal (ex: "hello") */
    NODE_ID,            /* Identificador/variável (ex: x) */
    NODE_BINOP,         /* Operador binário (ex: + - * /) */
    NODE_UNARYOP,       /* Operador unário (ex: - not) */
    NODE_CALL,          /* Chamada de função (ex: (+ 1 2)) */
    NODE_DEFINE,        /* Define (ex: (define x 5)) */
    NODE_IF,            /* If (ex: (if cond then else)) */
    NODE_COND,          /* Cond (ex: (cond (test1 exp1) (test2 exp2))) */
    NODE_LAMBDA,        /* Lambda (ex: (lambda (x) (+ x 1))) */
    NODE_LET,           /* Let (ex: (let ((x 1)) expr)) */
    NODE_BEGIN,         /* Begin (ex: (begin expr1 expr2)) */
    NODE_SET,           /* Set! (ex: (set! x 5)) */
    NODE_QUOTE,         /* Quote (ex: 'x) */
    NODE_COND_CLAUSE    /* Clausula de cond (test => result) */
} ast_node_type;

/* Estrutura para lista de nós AST (LSE) */
typedef struct ast_node_list {
    struct ast_node *node;          /* Nó AST */
    struct ast_node_list *next;     /* Próximo nó da lista */
} ast_node_list;

/* Estrutura principal do nó AST */
typedef struct ast_node {
    ast_node_type type;             /* Tipo de nó */
    value_type tipo_valor;          /* Tipo de valor para análise semântica */
    int linha;                      /* Número da linha no arquivo Scheme */
    
    union {
        /* NODE_NUM */
        double valor_num;
        
        /* NODE_STR */
        char *valor_str;
        
        /* NODE_ID */
        struct {
            char *nome;
        } id;
        
        /* NODE_BINOP */
        struct {
            char *op;               /* Operador: "+", "-", "*", "/", ">", "<", "=", etc */
            struct ast_node *left;
            struct ast_node *right;
        } binop;
        
        /* NODE_UNARYOP */
        struct {
            char *op;               /* Operador: "-", "not" */
            struct ast_node *operando;
        } unaryop;
        
        /* NODE_CALL */
        struct {
            char *funcao;           /* Nome da função */
            ast_node_list *argumentos;  /* LSE de argumentos */
        } call;
        
        /* NODE_DEFINE */
        struct {
            char *variavel;
            struct ast_node *valor;
        } define;
        
        /* NODE_IF */
        struct {
            struct ast_node *condicao;
            struct ast_node *then_branch;
            struct ast_node *else_branch;
        } if_node;
        
        /* NODE_LAMBDA */
        struct {
            ast_node_list *parametros;  /* LSE de nomes (strings) */
            struct ast_node *corpo;
        } lambda;
        
        /* NODE_LET */
        struct {
            ast_node_list *bindings;    /* LSE de (nome, valor) */
            struct ast_node *corpo;
            int is_letrec;              /* 1 se for letrec, 0 se for let */
        } let_node;
        
        /* NODE_BEGIN */
        struct {
            ast_node_list *expressoes;  /* LSE de expressões */
        } begin;
        
        /* NODE_SET */
        struct {
            char *variavel;
            struct ast_node *valor;
        } set;
        
        /* NODE_COND */
        struct {
            ast_node_list *clausulas;   /* LSE de NODE_COND_CLAUSE */
        } cond;
        
        /* NODE_COND_CLAUSE */
        struct {
            struct ast_node *teste;
            struct ast_node *resultado;
        } cond_clause;
        
        /* NODE_QUOTE */
        struct {
            struct ast_node *quoted;
        } quote;
        
    } data;
    
} ast_node;

/* ============================================================================
   FUNÇÕES CONSTRUTORAS E AUXILIARES
   ============================================================================ */

/* Construtores de nós */
ast_node *create_num(double valor);
ast_node *create_str(char *valor);
ast_node *create_id(char *nome);
ast_node *create_binop(char *op, ast_node *left, ast_node *right);
ast_node *create_unaryop(char *op, ast_node *operando);
ast_node *create_call(char *funcao, ast_node_list *argumentos);
ast_node *create_define(char *variavel, ast_node *valor);
ast_node *create_if(ast_node *condicao, ast_node *then_br, ast_node *else_br);
ast_node *create_lambda(ast_node_list *parametros, ast_node *corpo);
ast_node *create_let(ast_node_list *bindings, ast_node *corpo, int is_letrec);
ast_node *create_begin(ast_node_list *expressoes);
ast_node *create_set(char *variavel, ast_node *valor);
ast_node *create_cond(ast_node_list *clausulas);
ast_node *create_cond_clause(ast_node *teste, ast_node *resultado);
ast_node *create_quote(ast_node *quoted);

/* Gerenciamento de lista de nós */
ast_node_list *create_arg_node(ast_node *node);
ast_node_list *append_arg_node(ast_node_list *lista, ast_node *node);
void free_arg_list(ast_node_list *lista);

/* Desalocação */
void free_ast(ast_node *node);

/* Code generation */
char *codegen(ast_node *node);
char *codegen_args(ast_node_list *args, char *sep);

/* Type checking e análise semântica */
int check_types(ast_node *node);  /* Retorna 0 se erro, 1 se OK */
int type_inference(ast_node *node);  /* Infere tipo de uma expressão */
void report_type_error(const char *message, int linha);

#endif