#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union content{                                  
    double var;                                 
    double (*fnptr)(double);                    
};

typedef struct symrec {                         
    char *name;                                 
    int type;                                   
    union content value;                        
    int linha_declaracao;                       
    int foi_usado;                              
    int foi_usado_como;                         
    int eh_builtin;                             
    int foi_declarado;                          
    struct symrec *next;                        
} symrec;

extern symrec *sym_table;                       

symrec *putsym(char const *name, int type);     
symrec *getsym(char const *name);               
void inicializar_tabela();                      
void marcar_como_usado(const char *name);       
void verificar_nao_utilizados(void);            
void relatorio_erro_nao_declarado(const char *name, int linha);  

typedef enum {
    T_NUM,      
    T_STR,      
    T_BOOL,     
    T_FUNC,     
    T_ANY,      
    T_ERROR     
} value_type;

typedef enum {
    NODE_NUM,           
    NODE_STR,           
    NODE_ID,            
    NODE_BINOP,         
    NODE_UNARYOP,       
    NODE_CALL,         
    NODE_DEFINE,        
    NODE_IF,            
    NODE_COND,         
    NODE_LAMBDA,        
    NODE_LET,           
    NODE_BEGIN,         
    NODE_SET,           
    NODE_QUOTE,
    NODE_COND_CLAUSE
} ast_node_type;

typedef struct ast_node_list {
    struct ast_node *node;          
    struct ast_node_list *next;
} ast_node_list;

typedef struct ast_node {
    ast_node_type type;
    value_type tipo_valor;
    int linha;
    
    union {
        double valor_num;

        char *valor_str;
        
        struct {
            char *nome;
        } id;
        
        struct {
            char *op;            
            struct ast_node *left;
            struct ast_node *right;
        } binop;
        
        struct {
            char *op;               
            struct ast_node *operando;
        } unaryop;
        
        struct {
            char *funcao;           
            ast_node_list *argumentos;  
        } call;
        
        struct {
            char *variavel;
            struct ast_node *valor;
        } define;
        
        struct {
            struct ast_node *condicao;
            struct ast_node *then_branch;
            struct ast_node *else_branch;
        } if_node;
        
        struct {
            ast_node_list *parametros;  
            struct ast_node *corpo;
        } lambda;
        
        struct {
            ast_node_list *bindings;    
            struct ast_node *corpo;
            int is_letrec;              
        } let_node;
        
        struct {
            ast_node_list *expressoes; 
        } begin;
        
        struct {
            char *variavel;
            struct ast_node *valor;
        } set;
        
        struct {
            ast_node_list *clausulas;   
        } cond;
        
        struct {
            struct ast_node *teste;
            struct ast_node *resultado;
        } cond_clause;
        
        struct {
            struct ast_node *quoted;
        } quote;
        
    } data;
    
} ast_node;

ast_node *create_num(double valor);
ast_node *create_str(char *valor);
ast_node *create_id(char *nome);
ast_node *create_binop(char *op, ast_node *left, ast_node *right);
ast_node *create_unaryop(char *op, ast_node *operando);
ast_node *create_call(char *funcao, ast_node_list *argumentos);
ast_node *create_define(char *variavel, ast_node *valor);
ast_node *create_if(ast_node *condicao, ast_node *then_br, ast_node *else_br);
ast_node *create_lambda(ast_node_list *parametros, ast_node *corpo);
ast_node *create_let(ast_node_list *bindings, ast_node *corpo);
ast_node *create_begin(ast_node_list *expressoes);
ast_node *create_set(char *variavel, ast_node *valor);
ast_node *create_cond(ast_node_list *clausulas);
ast_node *create_cond_clause(ast_node *teste, ast_node *resultado);
ast_node *create_quote(ast_node *quoted);

ast_node_list *create_arg_node(ast_node *node);
ast_node_list *append_arg_node(ast_node_list *lista, ast_node *node);
void free_arg_list(ast_node_list *lista);

void free_ast(ast_node *node);

char *codegen(ast_node *node);
char *codegen_args(ast_node_list *args, char *sep);

int check_types(ast_node *node);  
int type_inference(ast_node *node); 
void report_type_error(const char *message, int linha);

#endif