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
     Ident = 258,
     Read = 259,
     IntLit = 260,
     Int = 261,
     Write = 262,
     IF = 263,
     ELSE = 264,
     EQ = 265,
     NOTEQ = 266,
     LTE = 267,
     GTE = 268,
     AMP = 269,
     OR = 270,
     EXPNT = 271,
     PrintLine = 272,
     PrintSpaces = 273,
     WHILE = 274,
     FOR = 275,
     VOID = 276,
     RETURN = 277
   };
#endif
/* Tokens.  */
#define Ident 258
#define Read 259
#define IntLit 260
#define Int 261
#define Write 262
#define IF 263
#define ELSE 264
#define EQ 265
#define NOTEQ 266
#define LTE 267
#define GTE 268
#define AMP 269
#define OR 270
#define EXPNT 271
#define PrintLine 272
#define PrintSpaces 273
#define WHILE 274
#define FOR 275
#define VOID 276
#define RETURN 277




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 22 "ExprEval.y"
{
  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct BExprRes * BExprRes;
}
/* Line 1529 of yacc.c.  */
#line 101 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

