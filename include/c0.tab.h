/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_SRC_C0_TAB_H_INCLUDED
# define YY_YY_SRC_C0_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    MAIN = 258,
    CONST = 259,
    INT = 260,
    VOID = 261,
    IF = 262,
    WHILE = 263,
    IDENT = 264,
    NUMBER = 265,
    EQL = 266,
    GRE = 267,
    GREQ = 268,
    LES = 269,
    LEQ = 270,
    NEQ = 271,
    PLUS = 272,
    MINUS = 273,
    MULT = 274,
    DIV = 275,
    LP = 276,
    RP = 277,
    LB = 278,
    RB = 279,
    ASSIGN = 280,
    COMMA = 281,
    SEMI = 282,
    MUIT = 283,
    UMINUS = 284
  };
#endif
/* Tokens.  */
#define MAIN 258
#define CONST 259
#define INT 260
#define VOID 261
#define IF 262
#define WHILE 263
#define IDENT 264
#define NUMBER 265
#define EQL 266
#define GRE 267
#define GREQ 268
#define LES 269
#define LEQ 270
#define NEQ 271
#define PLUS 272
#define MINUS 273
#define MULT 274
#define DIV 275
#define LP 276
#define RP 277
#define LB 278
#define RB 279
#define ASSIGN 280
#define COMMA 281
#define SEMI 282
#define MUIT 283
#define UMINUS 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 16 "config/c0.y" /* yacc.c:1909  */

   int val;
   char *name;
   ASTNode node;

#line 118 "src/c0.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SRC_C0_TAB_H_INCLUDED  */
