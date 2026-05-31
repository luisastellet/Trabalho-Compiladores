#include "scheme.h"

/* Acesso ao arquivo de saída Python (definido em sintatico.y) */
extern FILE *arquivo_python;

/* ============================================================================
   VARIÁVEIS GLOBAIS PARA TYPE CHECKING
   ============================================================================ */
static int type_errors_count = 0;
#define MAX_TYPE_ERRORS 100
static char *type_error_messages[MAX_TYPE_ERRORS];

/* Contador global para gerar nomes únicos de funções */
static int func_counter = 0;



ast_node *create_num(double valor) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_NUM;
    node->tipo_valor = T_NUM;
    node->linha = yylineno;
    node->data.valor_num = valor;
    return node;
}

ast_node *create_str(char *valor) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_STR;
    node->tipo_valor = T_STR;
    node->linha = yylineno;
    node->data.valor_str = strdup(valor);
    return node;
}

ast_node *create_id(char *nome) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_ID;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.id.nome = strdup(nome);
    return node;
}

ast_node *create_binop(char *op, ast_node *left, ast_node *right) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_BINOP;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.binop.op = strdup(op);
    node->data.binop.left = left;
    node->data.binop.right = right;
    return node;
}

ast_node *create_unaryop(char *op, ast_node *operando) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_UNARYOP;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.unaryop.op = strdup(op);
    node->data.unaryop.operando = operando;
    return node;
}

ast_node *create_call(char *funcao, ast_node_list *argumentos) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_CALL;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.call.funcao = strdup(funcao);
    node->data.call.argumentos = argumentos;
    return node;
}

ast_node *create_define(char *variavel, ast_node *valor) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_DEFINE;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.define.variavel = strdup(variavel);
    node->data.define.valor = valor;
    return node;
}

ast_node *create_if(ast_node *condicao, ast_node *then_br, ast_node *else_br) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_IF;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.if_node.condicao = condicao;
    node->data.if_node.then_branch = then_br;
    node->data.if_node.else_branch = else_br;
    return node;
}

ast_node *create_lambda(ast_node_list *parametros, ast_node *corpo) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_LAMBDA;
    node->tipo_valor = T_FUNC;
    node->linha = yylineno;
    node->data.lambda.parametros = parametros;
    node->data.lambda.corpo = corpo;
    return node;
}

ast_node *create_let(ast_node_list *bindings, ast_node *corpo) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_LET;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.let_node.bindings = bindings;
    node->data.let_node.corpo = corpo;
    return node;
}

ast_node *create_begin(ast_node_list *expressoes) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_BEGIN;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.begin.expressoes = expressoes;
    return node;
}

ast_node *create_set(char *variavel, ast_node *valor) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_SET;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.set.variavel = strdup(variavel);
    node->data.set.valor = valor;
    return node;
}

ast_node *create_cond(ast_node_list *clausulas) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_COND;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.cond.clausulas = clausulas;
    return node;
}

ast_node *create_cond_clause(ast_node *teste, ast_node *resultado) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_COND_CLAUSE;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.cond_clause.teste = teste;
    node->data.cond_clause.resultado = resultado;
    return node;
}

ast_node *create_quote(ast_node *quoted) {
    extern int yylineno;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = NODE_QUOTE;
    node->tipo_valor = T_ANY;
    node->linha = yylineno;
    node->data.quote.quoted = quoted;
    return node;
}

/* ============================================================================
   GERENCIAMENTO DE LISTAS DE NÓS
   ============================================================================ */

ast_node_list *create_arg_node(ast_node *node) {
    ast_node_list *arg = (ast_node_list *)malloc(sizeof(ast_node_list));
    arg->node = node;
    arg->next = NULL;
    return arg;
}

ast_node_list *append_arg_node(ast_node_list *lista, ast_node *node) {
    if (lista == NULL) {
        return create_arg_node(node);
    }
    
    ast_node_list *current = lista;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = create_arg_node(node);
    return lista;
}

void free_arg_list(ast_node_list *lista) {
    while (lista != NULL) {
        ast_node_list *temp = lista;
        lista = lista->next;
        free(temp);
    }
}

/* ============================================================================
   DESALOCAÇÃO
   ============================================================================ */

void free_ast(ast_node *node) {
    if (node == NULL) return;
    
    switch (node->type) {
        case NODE_NUM:
            /* Nada a desalocar */
            break;
            
        case NODE_STR:
            free(node->data.valor_str);
            break;
            
        case NODE_ID:
            free(node->data.id.nome);
            break;
            
        case NODE_BINOP:
            free(node->data.binop.op);
            free_ast(node->data.binop.left);
            free_ast(node->data.binop.right);
            break;
            
        case NODE_UNARYOP:
            free(node->data.unaryop.op);
            free_ast(node->data.unaryop.operando);
            break;
            
        case NODE_CALL:
            free(node->data.call.funcao);
            free_arg_list(node->data.call.argumentos);
            break;
            
        case NODE_DEFINE:
            free(node->data.define.variavel);
            free_ast(node->data.define.valor);
            break;
            
        case NODE_IF:
            free_ast(node->data.if_node.condicao);
            free_ast(node->data.if_node.then_branch);
            free_ast(node->data.if_node.else_branch);
            break;
            
        case NODE_LAMBDA:
            free_arg_list(node->data.lambda.parametros);
            free_ast(node->data.lambda.corpo);
            break;
            
        case NODE_LET:
            free_arg_list(node->data.let_node.bindings);
            free_ast(node->data.let_node.corpo);
            break;
            
        case NODE_BEGIN:
            free_arg_list(node->data.begin.expressoes);
            break;
            
        case NODE_SET:
            free(node->data.set.variavel);
            free_ast(node->data.set.valor);
            break;
            
        case NODE_COND:
            free_arg_list(node->data.cond.clausulas);
            break;
            
        case NODE_COND_CLAUSE:
            free_ast(node->data.cond_clause.teste);
            free_ast(node->data.cond_clause.resultado);
            break;
            
        case NODE_QUOTE:
            free_ast(node->data.quote.quoted);
            break;
    }
    
    free(node);
}

/* ============================================================================
   CODE GENERATION
   ============================================================================ */

/* Helper: converte operador Scheme para Python */
char *translate_operator(char *op) {
    if (strcmp(op, "=") == 0) return "==";
    if (strcmp(op, "not") == 0) return "not";
    return op; /* Maioria dos operadores são iguais em Python */
}

/* Helper: gera Python para lista de argumentos com separador */
char *codegen_args(ast_node_list *args, char *sep) {
    if (args == NULL) {
        char *result = malloc(1);
        result[0] = '\0';
        return result;
    }
    
    char *code = codegen(args->node);
    ast_node_list *current = args->next;
    
    while (current != NULL) {
        char *arg_code = codegen(current->node);
        char *temp = malloc(strlen(code) + strlen(sep) + strlen(arg_code) + 1);
        sprintf(temp, "%s%s%s", code, sep, arg_code);
        free(code);
        free(arg_code);
        code = temp;
        current = current->next;
    }
    
    return code;
}

/* Função auxiliar: sanitizar identificadores para Python (hífen → underscore) */
char *sanitize_identifier(const char *name) {
    if (name == NULL) return NULL;
    
    char *sanitized = strdup(name);
    for (int i = 0; sanitized[i] != '\0'; i++) {
        if (sanitized[i] == '-') {
            sanitized[i] = '_';
        }
    }
    return sanitized;
}

/* Função principal de geração de código */
char *codegen(ast_node *node) {
    if (node == NULL) return strdup("");
    
    char *result = NULL;
    char buffer[256];
    
    switch (node->type) {
        case NODE_NUM:
            result = malloc(32);
            sprintf(result, "%g", node->data.valor_num);
            break;
            
        case NODE_STR:
            result = malloc(strlen(node->data.valor_str) + 3);
            sprintf(result, "\"%s\"", node->data.valor_str);
            break;
            
        case NODE_ID: {
            char *sanitized = sanitize_identifier(node->data.id.nome);
            result = sanitized;
            break;
        }
            
        case NODE_BINOP: {
            char *left = codegen(node->data.binop.left);
            char *right = codegen(node->data.binop.right);
            char *op = translate_operator(node->data.binop.op);
            
            result = malloc(strlen(left) + strlen(right) + strlen(op) + 5);
            sprintf(result, "(%s %s %s)", left, op, right);
            free(left);
            free(right);
            break;
        }
        
        case NODE_UNARYOP: {
            char *operando = codegen(node->data.unaryop.operando);
            char *op = translate_operator(node->data.unaryop.op);
            
            result = malloc(strlen(operando) + strlen(op) + 3);
            sprintf(result, "(%s %s)", op, operando);
            free(operando);
            break;
        }
        
        case NODE_CALL: {
            char *args_code = codegen_args(node->data.call.argumentos, ", ");
            char *func_sanitized = sanitize_identifier(node->data.call.funcao);
            
            /* Tratamento especial para 'print' (traduzido de 'display')
               display em Scheme NÃO adiciona quebra de linha, ao contrário de print() em Python */
            if (strcmp(node->data.call.funcao, "print") == 0) {
                result = malloc(strlen(func_sanitized) + strlen(args_code) + 20);
                sprintf(result, "%s(%s, end='')", func_sanitized, args_code);
            } else {
                result = malloc(strlen(func_sanitized) + strlen(args_code) + 3);
                sprintf(result, "%s(%s)", func_sanitized, args_code);
            }
            
            free(args_code);
            free(func_sanitized);
            break;
        }
        
        case NODE_DEFINE: {
            char *valor = codegen(node->data.define.valor);
            char *var_sanitized = sanitize_identifier(node->data.define.variavel);
            
            result = malloc(strlen(var_sanitized) + strlen(valor) + 5);
            sprintf(result, "%s = %s", var_sanitized, valor);
            free(valor);
            free(var_sanitized);
            break;
        }
        
        case NODE_IF: {
            char *cond = codegen(node->data.if_node.condicao);
            char *then_br = codegen(node->data.if_node.then_branch);
            char *else_br = codegen(node->data.if_node.else_branch);
            
            result = malloc(strlen(then_br) + strlen(cond) + strlen(else_br) + 15);
            sprintf(result, "(%s if %s else %s)", then_br, cond, else_br);
            free(cond);
            free(then_br);
            free(else_br);
            break;
        }
        
        case NODE_LAMBDA: {
            /* (lambda (x y z) corpo) → def _lambda_N(x, y, z): return corpo
               Retorna apenas o nome da função (não a chama)
            */
            char func_name[64];
            snprintf(func_name, sizeof(func_name), "_lambda_%d", func_counter++);
            
            char *params = codegen_args(node->data.lambda.parametros, ", ");
            char *corpo = codegen(node->data.lambda.corpo);
            
            /* Gera a definição da função no arquivo de saída */
            fprintf(arquivo_python, "def %s(%s):\n    return %s\n\n", func_name, params, corpo);
            
            /* Retorna apenas o nome da função */
            result = malloc(strlen(func_name) + 1);
            strcpy(result, func_name);
            free(params);
            free(corpo);
            break;
        }
        
        case NODE_LET: {
            /* (let ((x 1) (y 2)) corpo) → def _let_N(): x=1; y=2; return corpo
                                               _let_N()
            */
            if (node->data.let_node.bindings == NULL) {
                char *corpo = codegen(node->data.let_node.corpo);
                result = corpo;
                break;
            }
            
            char func_name[64];
            snprintf(func_name, sizeof(func_name), "_let_%d", func_counter++);
            
            /* Gera statements (assignments e defs) para todas as bindings */
            char statements[4096];
            statements[0] = '\0';
            
            ast_node_list *binding = node->data.let_node.bindings;
            while (binding != NULL && binding->node != NULL) {
                if (binding->node->type == NODE_DEFINE) {
                    char *var_sanitized = sanitize_identifier(binding->node->data.define.variavel);
                    
                    /* Se o valor é um lambda, converte para def */
                    if (binding->node->data.define.valor && 
                        binding->node->data.define.valor->type == NODE_LAMBDA) {
                        
                        ast_node *lambda_node = binding->node->data.define.valor;
                        char *params = codegen_args(lambda_node->data.lambda.parametros, ", ");
                        char *corpo_lambda = codegen(lambda_node->data.lambda.corpo);
                        
                        char def_line[1024];
                        snprintf(def_line, sizeof(def_line), "    def %s(%s):\n        return %s\n",
                                 var_sanitized, params, corpo_lambda);
                        strcat(statements, def_line);
                        
                        free(params);
                        free(corpo_lambda);
                    } else {
                        /* Caso não-lambda, trata como atribuição normal */
                        char *valor_code = codegen(binding->node->data.define.valor);
                        char assign_line[512];
                        snprintf(assign_line, sizeof(assign_line), "    %s = %s\n",
                                 var_sanitized, valor_code);
                        strcat(statements, assign_line);
                        free(valor_code);
                    }
                    
                    free(var_sanitized);
                }
                binding = binding->next;
            }
            
            /* Gera o corpo (última expressão) */
            char *corpo_expr = codegen(node->data.let_node.corpo);
            
            /* Se o corpo é um NODE_DEFINE, extrai apenas a expressão */
            if (node->data.let_node.corpo && node->data.let_node.corpo->type == NODE_DEFINE) {
                char *valor_define = codegen(node->data.let_node.corpo->data.define.valor);
                free(corpo_expr);
                corpo_expr = valor_define;
            }
            
            /* Escreve a função no arquivo de saída */
            fprintf(arquivo_python, "def %s():\n%s    return %s\n\n", func_name, statements, corpo_expr);
            
            /* Retorna a chamada da função */
            result = malloc(strlen(func_name) + 4);
            sprintf(result, "%s()", func_name);
            free(corpo_expr);
            
            break;
        }
        
        case NODE_BEGIN: {
            /* (begin expr1 expr2 expr3) 
               Em Python pode ser: expr1; expr2; expr3
               Ou em um bloco. Por enquanto, concatena com vírgula/ponto-e-vírgula
            */
            char *exprs = codegen_args(node->data.begin.expressoes, "; ");
            
            result = malloc(strlen(exprs) + 3);
            sprintf(result, "(%s)", exprs);
            free(exprs);
            break;
        }
        
        case NODE_SET: {
            /* (set! variavel valor) → variavel = valor */
            char *valor = codegen(node->data.set.valor);
            
            result = malloc(strlen(node->data.set.variavel) + strlen(valor) + 5);
            sprintf(result, "%s = %s", node->data.set.variavel, valor);
            free(valor);
            break;
        }
        
        case NODE_COND: {
            /* (cond (teste1 expr1) (teste2 expr2) (else expr3))
               Traduz para: expr1 if teste1 else (expr2 if teste2 else expr3)
               Ternários aninhados em Python
            */
            if (node->data.cond.clausulas == NULL) {
                result = strdup("None");
                break;
            }
            
            /* Primeiro: conta as cláusulas e coloca em um array para processar de trás para frente */
            ast_node_list *clausulas = node->data.cond.clausulas;
            int num_clausulas = 0;
            ast_node_list *temp = clausulas;
            
            while (temp != NULL) {
                num_clausulas++;
                temp = temp->next;
            }
            
            if (num_clausulas == 0) {
                result = strdup("None");
                break;
            }
            
            /* Array de ponteiros para as cláusulas */
            ast_node **clauses = (ast_node **)malloc(num_clausulas * sizeof(ast_node *));
            temp = clausulas;
            for (int i = 0; i < num_clausulas; i++) {
                clauses[i] = temp->node;
                temp = temp->next;
            }
            
            /* Processa de trás para frente (começando pela última) */
            /* Verifica se última é else */
            int last_is_else = 0;
            if (clauses[num_clausulas - 1]->type == NODE_COND_CLAUSE) {
                ast_node *teste_node = clauses[num_clausulas - 1]->data.cond_clause.teste;
                if (teste_node->type == NODE_ID && 
                    strcmp(teste_node->data.id.nome, "else") == 0) {
                    last_is_else = 1;
                }
            }
            
            /* Começa com a última cláusula */
            char *teste_str = codegen(clauses[num_clausulas - 1]->data.cond_clause.teste);
            char *resultado_str = codegen(clauses[num_clausulas - 1]->data.cond_clause.resultado);
            
            if (last_is_else) {
                result = resultado_str;
                free(teste_str);
            } else {
                result = malloc(strlen(resultado_str) + strlen(teste_str) + 15);
                sprintf(result, "%s if %s else None", resultado_str, teste_str);
                free(teste_str);
                free(resultado_str);
            }
            
            /* Processa todas as cláusulas anteriores, de trás para frente */
            for (int i = num_clausulas - 2; i >= 0; i--) {
                if (clauses[i]->type == NODE_COND_CLAUSE) {
                    char *t = codegen(clauses[i]->data.cond_clause.teste);
                    char *r = codegen(clauses[i]->data.cond_clause.resultado);
                    
                    char *novo = malloc(strlen(r) + strlen(t) + strlen(result) + 15);
                    sprintf(novo, "%s if %s else (%s)", r, t, result);
                    
                    free(t);
                    free(r);
                    free(result);
                    result = novo;
                }
            }
            
            free(clauses);
            break;
        }
        
        case NODE_QUOTE: {
            char *quoted = codegen(node->data.quote.quoted);
            
            result = malloc(strlen(quoted) + 2);
            sprintf(result, "'%s'", quoted);
            free(quoted);
            break;
        }
        
        case NODE_COND_CLAUSE:
        default:
            result = strdup("# Nó não implementado");
            break;
    }
    
    return result ? result : strdup("");
}

/* ============================================================================
   TYPE CHECKING E ANÁLISE SEMÂNTICA
   ============================================================================ */

void report_type_error(const char *message, int linha) {
    if (type_errors_count < MAX_TYPE_ERRORS) {
        type_error_messages[type_errors_count] = strdup(message);
        fprintf(stderr, "\nERRO DE TIPO na linha %d: %s\n", linha, message);
        type_errors_count++;
    }
}

int type_inference(ast_node *node) {
    if (node == NULL) return T_ANY;
    
    switch (node->type) {
        case NODE_NUM:
            return T_NUM;
        case NODE_STR:
            return T_STR;
        case NODE_LAMBDA:
            return T_FUNC;
        case NODE_ID:
            /* Tipo de variável é desconhecido sem análise de contexto */
            return T_ANY;
        case NODE_BINOP: {
            int left_type = type_inference(node->data.binop.left);
            int right_type = type_inference(node->data.binop.right);
            char *op = node->data.binop.op;
            
            /* Operadores aritméticos retornam NUM */
            if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || 
                strcmp(op, "*") == 0 || strcmp(op, "/") == 0) {
                return T_NUM;
            }
            /* Operadores de comparação retornam BOOL */
            if (strcmp(op, ">") == 0 || strcmp(op, "<") == 0 || 
                strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0 || 
                strcmp(op, "=") == 0) {
                return T_BOOL;
            }
            /* Operadores lógicos retornam BOOL */
            if (strcmp(op, "and") == 0 || strcmp(op, "or") == 0) {
                return T_BOOL;
            }
            return T_ANY;
        }
        case NODE_IF:
            /* Type do if é o type do then_branch ou else_branch */
            return type_inference(node->data.if_node.then_branch);
        case NODE_COND:
            /* Type do cond é o type de uma das cláusulas */
            if (node->data.cond.clausulas != NULL && 
                node->data.cond.clausulas->node != NULL) {
                return type_inference(node->data.cond.clausulas->node->data.cond_clause.resultado);
            }
            return T_ANY;
        case NODE_CALL:
            return T_ANY;  /* Tipo de chamada de função é desconhecido */
        case NODE_BEGIN:
            /* Type do begin é o type da última expressão */
            if (node->data.begin.expressoes != NULL) {
                ast_node_list *temp = node->data.begin.expressoes;
                ast_node *last = NULL;
                while (temp != NULL) {
                    last = temp->node;
                    temp = temp->next;
                }
                return type_inference(last);
            }
            return T_ANY;
        case NODE_LET:
            /* Type do let é o type do corpo */
            return type_inference(node->data.let_node.corpo);
        default:
            return T_ANY;
    }
}

int check_types(ast_node *node) {
    if (node == NULL) return 1;  /* OK */
    
    switch (node->type) {
        case NODE_NUM:
        case NODE_STR:
        case NODE_ID:
        case NODE_QUOTE:
            /* Tipos simples, OK */
            return 1;
            
        case NODE_BINOP: {
            char *op = node->data.binop.op;
            int left_type = type_inference(node->data.binop.left);
            int right_type = type_inference(node->data.binop.right);
            
            /* Validar tipos de operandos */
            if ((strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || 
                 strcmp(op, "*") == 0 || strcmp(op, "/") == 0) &&
                left_type != T_ANY && left_type != T_NUM) {
                char msg[256];
                sprintf(msg, "Operador '%s' esperava número, mas recebeu %s no operando esquerdo",
                        op, (left_type == T_STR ? "string" : "booleano"));
                report_type_error(msg, node->linha);
                return 0;
            }
            
            if ((strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || 
                 strcmp(op, "*") == 0 || strcmp(op, "/") == 0) &&
                right_type != T_ANY && right_type != T_NUM) {
                char msg[256];
                sprintf(msg, "Operador '%s' esperava número, mas recebeu %s no operando direito",
                        op, (right_type == T_STR ? "string" : "booleano"));
                report_type_error(msg, node->linha);
                return 0;
            }
            
            /* Operadores de comparação */
            if ((strcmp(op, ">") == 0 || strcmp(op, "<") == 0 || 
                 strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0) &&
                left_type != T_ANY && left_type != T_NUM) {
                char msg[256];
                sprintf(msg, "Operador '%s' esperava número, mas recebeu %s no operando esquerdo",
                        op, (left_type == T_STR ? "string" : "booleano"));
                report_type_error(msg, node->linha);
                return 0;
            }
            
            if ((strcmp(op, ">") == 0 || strcmp(op, "<") == 0 || 
                 strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0) &&
                right_type != T_ANY && right_type != T_NUM) {
                char msg[256];
                sprintf(msg, "Operador '%s' esperava número, mas recebeu %s no operando direito",
                        op, (right_type == T_STR ? "string" : "booleano"));
                report_type_error(msg, node->linha);
                return 0;
            }
            
            /* Recursivamente validar os operandos */
            if (!check_types(node->data.binop.left)) return 0;
            if (!check_types(node->data.binop.right)) return 0;
            
            return 1;
        }
        
        case NODE_UNARYOP: {
            int operand_type = type_inference(node->data.unaryop.operando);
            char *op = node->data.unaryop.op;
            
            if (strcmp(op, "-") == 0 && operand_type != T_ANY && operand_type != T_NUM) {
                char msg[256];
                sprintf(msg, "Operador unário '-' esperava número, mas recebeu %s",
                        (operand_type == T_STR ? "string" : "booleano"));
                report_type_error(msg, node->linha);
                return 0;
            }
            
            if (!check_types(node->data.unaryop.operando)) return 0;
            return 1;
        }
        
        case NODE_CALL: {
            /* Validar argumentos */
            ast_node_list *args = node->data.call.argumentos;
            while (args != NULL && args->node != NULL) {
                if (!check_types(args->node)) return 0;
                args = args->next;
            }
            return 1;
        }
        
        case NODE_IF: {
            if (!check_types(node->data.if_node.condicao)) return 0;
            if (!check_types(node->data.if_node.then_branch)) return 0;
            if (!check_types(node->data.if_node.else_branch)) return 0;
            return 1;
        }
        
        case NODE_COND: {
            ast_node_list *clausulas = node->data.cond.clausulas;
            while (clausulas != NULL && clausulas->node != NULL) {
                if (clausulas->node->type == NODE_COND_CLAUSE) {
                    if (!check_types(clausulas->node->data.cond_clause.teste)) return 0;
                    if (!check_types(clausulas->node->data.cond_clause.resultado)) return 0;
                }
                clausulas = clausulas->next;
            }
            return 1;
        }
        
        case NODE_LAMBDA: {
            /* Validar corpo da lambda */
            if (!check_types(node->data.lambda.corpo)) return 0;
            return 1;
        }
        
        case NODE_LET: {
            /* Validar bindings */
            ast_node_list *bindings = node->data.let_node.bindings;
            while (bindings != NULL && bindings->node != NULL) {
                if (bindings->node->type == NODE_DEFINE) {
                    if (!check_types(bindings->node->data.define.valor)) return 0;
                }
                bindings = bindings->next;
            }
            /* Validar corpo */
            if (!check_types(node->data.let_node.corpo)) return 0;
            return 1;
        }
        
        case NODE_BEGIN: {
            ast_node_list *exprs = node->data.begin.expressoes;
            while (exprs != NULL && exprs->node != NULL) {
                if (!check_types(exprs->node)) return 0;
                exprs = exprs->next;
            }
            return 1;
        }
        
        case NODE_DEFINE: {
            if (!check_types(node->data.define.valor)) return 0;
            return 1;
        }
        
        case NODE_SET: {
            if (!check_types(node->data.set.valor)) return 0;
            return 1;
        }
        
        default:
            return 1;
    }
}

int has_type_errors() {
    return type_errors_count > 0;
}

void clear_type_errors() {
    for (int i = 0; i < type_errors_count; i++) {
        free(type_error_messages[i]);
    }
    type_errors_count = 0;
}
