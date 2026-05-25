/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "sintatico.y"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 191 "sintatico.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 204 "sintatico.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   97

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  9
/* YYNRULES -- Number of rules.  */
#define YYNRULES  33
/* YYNRULES -- Number of states.  */
#define YYNSTATES  73

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    18,    22,
      27,    30,    33,    36,    40,    43,    46,    52,    58,    64,
      70,    73,    77,    82,    83,    85,    88,    93,    98,    99,
     101,   104,   105,   107
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      33,     0,    -1,    34,    -1,    33,    34,    -1,     3,    -1,
       4,    -1,     5,    -1,    30,    35,    31,    -1,     6,     5,
      34,    -1,     8,    34,    34,    34,    -1,    14,    39,    -1,
      15,    39,    -1,    18,    39,    -1,    19,     5,    34,    -1,
      22,    34,    -1,     5,    39,    -1,     7,    30,    39,    31,
      34,    -1,     9,    30,    37,    31,    34,    -1,    10,    30,
      37,    31,    34,    -1,    11,    30,    37,    31,    34,    -1,
      12,    40,    -1,    13,    34,    40,    -1,    30,     5,    34,
      31,    -1,    -1,    36,    -1,    37,    36,    -1,    30,    34,
      34,    31,    -1,    30,    16,    34,    31,    -1,    -1,    34,
      -1,    39,    34,    -1,    -1,    38,    -1,    40,    38,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    61,    61,    73,    89,    92,    96,   108,   116,   123,
     128,   136,   145,   150,   155,   160,   222,   239,   256,   273,
     290,   296,   307,   319,   322,   325,   332,   336,   345,   348,
     351,   358,   361,   364
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOKEN_NUM", "TOKEN_STRING_LITERAL",
  "TOKEN_ID", "TOKEN_DEFINE", "TOKEN_LAMBDA", "TOKEN_IF", "TOKEN_LET",
  "TOKEN_LET_STAR", "TOKEN_LETREC", "TOKEN_COND", "TOKEN_CASE",
  "TOKEN_AND", "TOKEN_OR", "TOKEN_ELSE", "TOKEN_ARROW", "TOKEN_BEGIN",
  "TOKEN_SET_BANG", "TOKEN_DO", "TOKEN_DELAY", "TOKEN_QUOTE_KEYWORD",
  "TOKEN_QUASIQUOTE", "TOKEN_UNQUOTE", "TOKEN_UNQUOTE_SPLICING",
  "TOKEN_DEFINE_SYNTAX", "TOKEN_LET_SYNTAX", "TOKEN_LETREC_SYNTAX",
  "TOKEN_SYNTAX_RULES", "TOKEN_LPAREN", "TOKEN_RPAREN", "$accept",
  "programa", "expressao", "comando_lista", "binding", "binding_list",
  "cond_clause", "lista_argumentos", "cond_clauses", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    32,    33,    33,    34,    34,    34,    34,    35,    35,
      35,    35,    35,    35,    35,    35,    35,    35,    35,    35,
      35,    35,    36,    37,    37,    37,    38,    38,    39,    39,
      39,    40,    40,    40
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     3,     4,
       2,     2,     2,     3,     2,     2,     5,     5,     5,     5,
       2,     3,     4,     0,     1,     2,     4,     4,     0,     1,
       2,     0,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     5,     6,     0,     0,     2,    28,     0,     0,
       0,     0,     0,     0,    31,     0,    28,    28,    28,     0,
       0,     0,     1,     3,    29,    15,     0,    28,     0,    23,
      23,    23,     0,    32,    20,    31,    10,    11,    12,     0,
      14,     7,    30,     8,     0,     0,     0,    24,     0,     0,
       0,     0,     0,    33,    21,    13,     0,     9,     0,     0,
      25,     0,     0,     0,     0,    16,     0,    17,    18,    19,
      27,    26,    22
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     5,    24,    21,    47,    48,    33,    25,    34
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -38
static const yytype_int8 yypact[] =
{
      37,   -38,   -38,   -38,    63,     3,   -38,    37,     9,   -21,
      37,   -11,    -3,     1,    20,    37,    37,    37,    37,    25,
      37,    24,   -38,   -38,   -38,    37,    37,    37,    37,    27,
      27,    27,    13,   -38,    20,    20,    37,    37,    37,    37,
     -38,   -38,   -38,   -38,    18,    37,    55,   -38,   -29,     4,
      16,    37,    37,   -38,    20,   -38,    37,   -38,    37,    37,
     -38,    37,    37,    32,    33,   -38,    34,   -38,   -38,   -38,
     -38,   -38,   -38
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -38,   -38,     0,   -38,   -37,    23,   -30,    70,    31
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       6,    46,    59,    22,    53,    23,     1,     2,     3,    27,
      28,    60,    60,    60,    26,    35,     1,     2,     3,    29,
      40,     1,     2,     3,    53,    42,    43,    30,    45,    51,
      39,    31,    52,     4,    46,    61,    42,    42,    42,    55,
       1,     2,     3,     4,    42,    57,    46,    62,     4,    56,
      32,    63,    64,    49,    50,    41,    65,    46,    66,    67,
      58,    68,    69,    70,    71,    72,    54,     4,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
       0,    18,    19,     0,     0,    20,    36,    37,    38,     0,
       0,     0,     0,     0,     0,     0,     0,    44
};

static const yytype_int8 yycheck[] =
{
       0,    30,    31,     0,    34,     5,     3,     4,     5,    30,
      10,    48,    49,    50,     5,    15,     3,     4,     5,    30,
      20,     3,     4,     5,    54,    25,    26,    30,    28,    16,
       5,    30,    32,    30,    30,    31,    36,    37,    38,    39,
       3,     4,     5,    30,    44,    45,    30,    31,    30,    31,
      30,    51,    52,    30,    31,    31,    56,    30,    58,    59,
       5,    61,    62,    31,    31,    31,    35,    30,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    -1,
      -1,    18,    19,    -1,    -1,    22,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,    30,    33,    34,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    18,    19,
      22,    35,     0,    34,    34,    39,     5,    30,    34,    30,
      30,    30,    30,    38,    40,    34,    39,    39,    39,     5,
      34,    31,    34,    34,    39,    34,    30,    36,    37,    37,
      37,    16,    34,    38,    40,    34,    31,    34,     5,    31,
      36,    31,    31,    34,    34,    34,    34,    34,    34,    34,
      31,    31,    31
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 34: /* "expressao" */
#line 54 "sintatico.y"
	{ free_ast((yyvaluep->node)); };
#line 1153 "sintatico.tab.c"
	break;
      case 35: /* "comando_lista" */
#line 54 "sintatico.y"
	{ free_ast((yyvaluep->node)); };
#line 1158 "sintatico.tab.c"
	break;
      case 36: /* "binding" */
#line 54 "sintatico.y"
	{ free_ast((yyvaluep->node)); };
#line 1163 "sintatico.tab.c"
	break;
      case 37: /* "binding_list" */
#line 55 "sintatico.y"
	{ free_arg_list((yyvaluep->args)); };
#line 1168 "sintatico.tab.c"
	break;
      case 38: /* "cond_clause" */
#line 54 "sintatico.y"
	{ free_ast((yyvaluep->node)); };
#line 1173 "sintatico.tab.c"
	break;
      case 39: /* "lista_argumentos" */
#line 55 "sintatico.y"
	{ free_arg_list((yyvaluep->args)); };
#line 1178 "sintatico.tab.c"
	break;
      case 40: /* "cond_clauses" */
#line 55 "sintatico.y"
	{ free_arg_list((yyvaluep->args)); };
#line 1183 "sintatico.tab.c"
	break;

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 61 "sintatico.y"
    { 
        /* Verificar tipos antes de gerar código */
        clear_type_errors();
        if (!check_types((yyvsp[(1) - (1)].node))) {
            fprintf(stderr, "\nCOMPILACAO ABORTADA: Erros de tipo detectados!\n\n");
            YYERROR;
        }
        
        char *codigo = codegen((yyvsp[(1) - (1)].node));
        fprintf(arquivo_python, "%s\n", codigo);
        free(codigo);
    ;}
    break;

  case 3:
#line 73 "sintatico.y"
    { 
        /* Verificar tipos antes de gerar código */
        clear_type_errors();
        if (!check_types((yyvsp[(2) - (2)].node))) {
            fprintf(stderr, "\nCOMPILACAO ABORTADA: Erros de tipo detectados!\n\n");
            YYERROR;
        }
        
        char *codigo = codegen((yyvsp[(2) - (2)].node));
        fprintf(arquivo_python, "%s\n", codigo);
        free(codigo);
    ;}
    break;

  case 4:
#line 89 "sintatico.y"
    {
        (yyval.node) = create_num((yyvsp[(1) - (1)].num));
    ;}
    break;

  case 5:
#line 92 "sintatico.y"
    {
        (yyval.node) = create_str((yyvsp[(1) - (1)].str));
        free((yyvsp[(1) - (1)].str));
    ;}
    break;

  case 6:
#line 96 "sintatico.y"
    {
        /* VERIFICAÇÃO DE CONTEXTO */
        symrec *s = getsym((yyvsp[(1) - (1)].ptr)->name);
        if (s == NULL) {
            /* Se não existe, cria uma entrada (pode ser parâmetro de lambda/let) */
            s = putsym((yyvsp[(1) - (1)].ptr)->name, TOKEN_ID);
            /* Marca como não declarado inicialmente; será marcado como declarado
               se for parâmetro de lambda/let */
        }
        marcar_como_usado((yyvsp[(1) - (1)].ptr)->name);  /* Marca como usado */
        (yyval.node) = create_id((yyvsp[(1) - (1)].ptr)->name);
    ;}
    break;

  case 7:
#line 108 "sintatico.y"
    {
        (yyval.node) = (yyvsp[(2) - (3)].node);
    ;}
    break;

  case 8:
#line 116 "sintatico.y"
    {
        symrec *s = putsym((yyvsp[(2) - (3)].ptr)->name, TOKEN_ID); // Registra no contexto
        s->foi_declarado = 1;  /* Marca como declarado */
        (yyval.node) = create_define((yyvsp[(2) - (3)].ptr)->name, (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 9:
#line 123 "sintatico.y"
    {
        (yyval.node) = create_if((yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node));
    ;}
    break;

  case 10:
#line 128 "sintatico.y"
    {
        /* (and a b c) -> (a and b and c) em Python */
        char *codigo = codegen_args((yyvsp[(2) - (2)].args), " and ");
        (yyval.node) = create_id(codigo); /* Simplificado: retorna como ID */
        free(codigo);
        free_arg_list((yyvsp[(2) - (2)].args));
    ;}
    break;

  case 11:
#line 136 "sintatico.y"
    {
        /* (or a b c) -> (a or b or c) em Python */
        char *codigo = codegen_args((yyvsp[(2) - (2)].args), " or ");
        (yyval.node) = create_id(codigo); /* Simplificado: retorna como ID */
        free(codigo);
        free_arg_list((yyvsp[(2) - (2)].args));
    ;}
    break;

  case 12:
#line 145 "sintatico.y"
    {
        (yyval.node) = create_begin((yyvsp[(2) - (2)].args));
    ;}
    break;

  case 13:
#line 150 "sintatico.y"
    {
        (yyval.node) = create_set((yyvsp[(2) - (3)].ptr)->name, (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 14:
#line 155 "sintatico.y"
    {
        (yyval.node) = create_quote((yyvsp[(2) - (2)].node));
    ;}
    break;

  case 15:
#line 160 "sintatico.y"
    {
        /* Aqui diferenciamos operadores especiais de chamadas de função */
        
        /* Garante que o identificador existe na tabela de símbolos */
        symrec *s = getsym((yyvsp[(1) - (2)].ptr)->name);
        if (s == NULL) {
            s = putsym((yyvsp[(1) - (2)].ptr)->name, TOKEN_ID);
        }
        
        marcar_como_usado((yyvsp[(1) - (2)].ptr)->name);  /* Marca identificador como usado */
        
        /* 1. OPERADORES ARITMÉTICOS INFIXOS */
        if (strcmp((yyvsp[(1) - (2)].ptr)->name, "+") == 0 || strcmp((yyvsp[(1) - (2)].ptr)->name, "-") == 0 || 
            strcmp((yyvsp[(1) - (2)].ptr)->name, "*") == 0 || strcmp((yyvsp[(1) - (2)].ptr)->name, "/") == 0) {
            
            /* Converte LSE de argumentos para binop aninhado (esquerda-associativo) */
            if ((yyvsp[(2) - (2)].args) == NULL) {
                yyerror("Operador matemático requer argumentos");
                YYERROR;
            }
            
            ast_node *result = (yyvsp[(2) - (2)].args)->node;
            ast_node_list *current = (yyvsp[(2) - (2)].args)->next;
            
            while (current != NULL) {
                result = create_binop((yyvsp[(1) - (2)].ptr)->name, result, current->node);
                current = current->next;
            }
            
            free_arg_list((yyvsp[(2) - (2)].args));
            (yyval.node) = result;
        }
        /* 2. OPERADORES COMPARAÇÃO INFIXOS */
        else if (strcmp((yyvsp[(1) - (2)].ptr)->name, ">") == 0 || strcmp((yyvsp[(1) - (2)].ptr)->name, "<") == 0 || 
                 strcmp((yyvsp[(1) - (2)].ptr)->name, ">=") == 0 || strcmp((yyvsp[(1) - (2)].ptr)->name, "<=") == 0 ||
                 strcmp((yyvsp[(1) - (2)].ptr)->name, "=") == 0) {
            
            if ((yyvsp[(2) - (2)].args) == NULL || (yyvsp[(2) - (2)].args)->next == NULL) {
                yyerror("Operador de comparação requer exatamente 2 argumentos");
                YYERROR;
            }
            
            (yyval.node) = create_binop((yyvsp[(1) - (2)].ptr)->name, (yyvsp[(2) - (2)].args)->node, (yyvsp[(2) - (2)].args)->next->node);
            free_arg_list((yyvsp[(2) - (2)].args));
        }
        /* 3. OPERADOR DISPLAY (traduz para print) */
        else if (strcmp((yyvsp[(1) - (2)].ptr)->name, "display") == 0) {
            if ((yyvsp[(2) - (2)].args) == NULL) {
                yyerror("display requer um argumento");
                YYERROR;
            }
            
            /* Cria nó de chamada com nome "print" */
            (yyval.node) = create_call("print", (yyvsp[(2) - (2)].args));
        }
        /* 4. PRIMITIVAS GENÉRICAS E FUNÇÕES DO USUÁRIO */
        else {
            (yyval.node) = create_call((yyvsp[(1) - (2)].ptr)->name, (yyvsp[(2) - (2)].args));
        }
    ;}
    break;

  case 16:
#line 222 "sintatico.y"
    {
        /* (lambda (x y z) corpo) */
        /* Marca parâmetros como declarados */
        ast_node_list *param = (yyvsp[(3) - (5)].args);
        while (param != NULL) {
            if (param->node->type == NODE_ID) {
                symrec *s = getsym(param->node->data.id.nome);
                if (s != NULL) {
                    s->foi_declarado = 1;
                }
            }
            param = param->next;
        }
        (yyval.node) = create_lambda((yyvsp[(3) - (5)].args), (yyvsp[(5) - (5)].node));
    ;}
    break;

  case 17:
#line 239 "sintatico.y"
    {
        /* (let ((x 1) (y 2)) corpo) */
        /* Marca variáveis de binding como declaradas */
        ast_node_list *binding = (yyvsp[(3) - (5)].args);
        while (binding != NULL) {
            if (binding->node->type == NODE_DEFINE) {
                symrec *s = getsym(binding->node->data.define.variavel);
                if (s != NULL) {
                    s->foi_declarado = 1;
                }
            }
            binding = binding->next;
        }
        (yyval.node) = create_let((yyvsp[(3) - (5)].args), (yyvsp[(5) - (5)].node));
    ;}
    break;

  case 18:
#line 256 "sintatico.y"
    {
        /* (let* ((x 1) (y 2)) corpo) - similar ao let por enquanto */
        /* Marca variáveis de binding como declaradas */
        ast_node_list *binding = (yyvsp[(3) - (5)].args);
        while (binding != NULL) {
            if (binding->node->type == NODE_DEFINE) {
                symrec *s = getsym(binding->node->data.define.variavel);
                if (s != NULL) {
                    s->foi_declarado = 1;
                }
            }
            binding = binding->next;
        }
        (yyval.node) = create_let((yyvsp[(3) - (5)].args), (yyvsp[(5) - (5)].node));
    ;}
    break;

  case 19:
#line 273 "sintatico.y"
    {
        /* (letrec ((x 1) (y 2)) corpo) - similar ao let por enquanto */
        /* Marca variáveis de binding como declaradas */
        ast_node_list *binding = (yyvsp[(3) - (5)].args);
        while (binding != NULL) {
            if (binding->node->type == NODE_DEFINE) {
                symrec *s = getsym(binding->node->data.define.variavel);
                if (s != NULL) {
                    s->foi_declarado = 1;
                }
            }
            binding = binding->next;
        }
        (yyval.node) = create_let((yyvsp[(3) - (5)].args), (yyvsp[(5) - (5)].node));
    ;}
    break;

  case 20:
#line 290 "sintatico.y"
    {
        /* (cond (teste1 expr1) (teste2 expr2) (else expr3)) */
        (yyval.node) = create_cond((yyvsp[(2) - (2)].args));
    ;}
    break;

  case 21:
#line 296 "sintatico.y"
    {
        /* (case expr (valor1 expr1) (valor2 expr2) ...) */
        /* TODO: Implementar pattern matching baseado em $2 */
        (void)(yyvsp[(2) - (3)].node); /* Silencia warning de variável não usada */
        (yyval.node) = create_cond((yyvsp[(3) - (3)].args));
    ;}
    break;

  case 22:
#line 307 "sintatico.y"
    {
        /* Cria um "pseudo-nó" que representa a binding
           Usando NODE_LET como container (não é o melhor, mas funciona)
           node->data.define.variavel = nome
           node->data.define.valor = expressão
        */
        (yyval.node) = create_define((yyvsp[(2) - (4)].ptr)->name, (yyvsp[(3) - (4)].node));
    ;}
    break;

  case 23:
#line 319 "sintatico.y"
    {
        (yyval.args) = NULL;
    ;}
    break;

  case 24:
#line 322 "sintatico.y"
    {
        (yyval.args) = create_arg_node((yyvsp[(1) - (1)].node));
    ;}
    break;

  case 25:
#line 325 "sintatico.y"
    {
        (yyval.args) = append_arg_node((yyvsp[(1) - (2)].args), (yyvsp[(2) - (2)].node));
    ;}
    break;

  case 26:
#line 332 "sintatico.y"
    {
        /* (teste resultado) */
        (yyval.node) = create_cond_clause((yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].node));
    ;}
    break;

  case 27:
#line 336 "sintatico.y"
    {
        /* (else resultado) - cria teste com valor "else" */
        ast_node *teste = create_id("else");
        (yyval.node) = create_cond_clause(teste, (yyvsp[(3) - (4)].node));
    ;}
    break;

  case 28:
#line 345 "sintatico.y"
    {
        (yyval.args) = NULL;
    ;}
    break;

  case 29:
#line 348 "sintatico.y"
    {
        (yyval.args) = create_arg_node((yyvsp[(1) - (1)].node));
    ;}
    break;

  case 30:
#line 351 "sintatico.y"
    {
        (yyval.args) = append_arg_node((yyvsp[(1) - (2)].args), (yyvsp[(2) - (2)].node));
    ;}
    break;

  case 31:
#line 358 "sintatico.y"
    {
        (yyval.args) = NULL;
    ;}
    break;

  case 32:
#line 361 "sintatico.y"
    {
        (yyval.args) = create_arg_node((yyvsp[(1) - (1)].node));
    ;}
    break;

  case 33:
#line 364 "sintatico.y"
    {
        (yyval.args) = append_arg_node((yyvsp[(1) - (2)].args), (yyvsp[(2) - (2)].node));
    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 1872 "sintatico.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 369 "sintatico.y"


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
