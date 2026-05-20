#ifndef CALC_H
#define CALC_H

union content{                                  /* Union serve como um OU, ou seja, o conteúdo é valor de variável OU uma função */
    double var;                                 /* Para armazenar valores de variáveis */
    double (*fnptr)(double);                    /* Para armazenar ponteiros de funções */ 
};

typedef struct symrec {                         /* Estrutura do Symbol Record*/
    char *name;                                 /* Nome do símbolo (ex: "x", "+", "define")*/
    int type;                                   /* Tipo do símbolo guardado (ex: "TOKEN_ID", "TOKEN_DEFINE")*/
    union content value;                        /* Valor do conteúdo, pela lógica do union pode ser de fato uma variável, ou uma função */
    struct symrec *next;                        /* Ponteiro para o próximo Símbolo, já que é uma LSE */
} symrec;

extern symrec *sym_table;                       /* Qualquer arquivo que tenha um "include scheme.h" podem acessar e modificar a sym_table, um ponteiro do tipo symrec*/

symrec *putsym(char const *name, int type);     /* Função para inserir um novo símbolo na tabela */
symrec *getsym(char const *name);               /* Função para buscar se um símbolo existe */
void inicializar_tabela();                      /* Função para inicializar a tabela de símbolos */

#endif