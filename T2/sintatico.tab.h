/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_NUM = 258,
     TOKEN_STRING_LITERAL = 259,
     TOKEN_ID = 260,
     TOKEN_DEFINE = 261,
     TOKEN_LAMBDA = 262,
     TOKEN_IF = 263,
     TOKEN_LET = 264,
     TOKEN_LET_STAR = 265,
     TOKEN_LETREC = 266,
     TOKEN_COND = 267,
     TOKEN_CASE = 268,
     TOKEN_AND = 269,
     TOKEN_OR = 270,
     TOKEN_ELSE = 271,
     TOKEN_ARROW = 272,
     TOKEN_BEGIN = 273,
     TOKEN_SET_BANG = 274,
     TOKEN_DO = 275,
     TOKEN_DELAY = 276,
     TOKEN_QUOTE_KEYWORD = 277,
     TOKEN_QUASIQUOTE = 278,
     TOKEN_UNQUOTE = 279,
     TOKEN_UNQUOTE_SPLICING = 280,
     TOKEN_DEFINE_SYNTAX = 281,
     TOKEN_LET_SYNTAX = 282,
     TOKEN_LETREC_SYNTAX = 283,
     TOKEN_SYNTAX_RULES = 284,
     TOKEN_LPAREN = 285,
     TOKEN_RPAREN = 286
   };
#endif
/* Tokens.  */
#define TOKEN_NUM 258
#define TOKEN_STRING_LITERAL 259
#define TOKEN_ID 260
#define TOKEN_DEFINE 261
#define TOKEN_LAMBDA 262
#define TOKEN_IF 263
#define TOKEN_LET 264
#define TOKEN_LET_STAR 265
#define TOKEN_LETREC 266
#define TOKEN_COND 267
#define TOKEN_CASE 268
#define TOKEN_AND 269
#define TOKEN_OR 270
#define TOKEN_ELSE 271
#define TOKEN_ARROW 272
#define TOKEN_BEGIN 273
#define TOKEN_SET_BANG 274
#define TOKEN_DO 275
#define TOKEN_DELAY 276
#define TOKEN_QUOTE_KEYWORD 277
#define TOKEN_QUASIQUOTE 278
#define TOKEN_UNQUOTE 279
#define TOKEN_UNQUOTE_SPLICING 280
#define TOKEN_DEFINE_SYNTAX 281
#define TOKEN_LET_SYNTAX 282
#define TOKEN_LETREC_SYNTAX 283
#define TOKEN_SYNTAX_RULES 284
#define TOKEN_LPAREN 285
#define TOKEN_RPAREN 286




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 26 "sintatico.y"
{
    ast_node *node;         /* Nó da AST */
    ast_node_list *args;    /* Lista de nós AST */
    char *str;              /* Para strings literal no lexer */
    double num;             /* Para números */
    symrec *ptr;            /* Para símbolos da tabela */
}
/* Line 1529 of yacc.c.  */
#line 119 "sintatico.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

