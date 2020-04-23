%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOMngr.h"
#include "SymTab.h"
#include "Semantics.h"
#include "CodeGen.h"

extern int yylex();	/* The next token function. */
extern char *yytext;   /* The matched token text.  */
extern int yyleng;      /* The token text length.   */
extern int yyparse();
extern int yyerror(char *);
void dumpTable();

extern SymTab *table;

%}


%union {
  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct BExprRes * BExprRes;
}

%type <string> Id
%type <ExprRes> Factor
%type <ExprRes> XFactor
%type <ExprRes> Term
%type <ExprRes> Expr
%type <ExprRes> Exprs
%type <ExprRes> Idents
%type <InstrSeq> StmtSeq
%type <InstrSeq> Stmt
%type <BExprRes> BExpr
%type <BExprRes> BStmt
%type <BExprRes> BFactor

%token Ident
%token Read
%token IntLit
%token Int
%token Write
%token IF
%token EQ
%token NOTEQ
%token LTE
%token GTE
%token AMP
%token OR
%token EXPNT
%token PrintLine
%token PrintSpaces


%%


Prog			    :	Declarations StmtSeq						          {Finish($2); } ;
Declarations	:	Dec Declarations							            {};
Declarations	:											                      {};
Dec			      :	Int Ident {enterName(table, yytext); }';'	{};
StmtSeq       :	Stmt StmtSeq								              {$$ = AppendSeq($1, $2); } ;
StmtSeq		    :											                      {$$ = NULL;} ;
Stmt			    :	Write Expr ';'								            {$$ = doPrint($2); };
Stmt          : Write '(' Exprs ')' ';'                   {$$ = addLine($3);};
Stmt          : Read '(' Idents ')' ';'                   {$$ = addLine($3); };
Stmt          : PrintLine ';'                             {$$ = doPrintLine();};
Stmt			    :	Id '=' Expr ';'								            {$$ = doAssign($1, $3);} ;
Stmt			    :	IF '(' BStmt ')' '{' StmtSeq '}'	        {$$ = doIf($3, $6);};
Stmt          : PrintSpaces '(' Expr ')'  ';'             {$$ = doPrintSpaces($3);};
BStmt         : '(' BStmt ')'                             {$$ = $2;};
BStmt         : BFactor                                   {$$ = $1;};
BFactor       : BExpr AMP BExpr                           {$$ = doAnd($1, $3);};
BFactor       : BExpr OR BExpr                            {$$ = doOr($1, $3);};
BFactor       : BExpr                                     {$$ = $1;};
BExpr         : '!' BExpr                                 {$$ = doNot($2);};
BExpr		      :	Expr EQ Expr								              {$$ = doBExpr($1, $3);};
BExpr		      :	Expr NOTEQ Expr								            {$$ = doFalseBExpr($1, $3);};
BExpr		      :	Expr '<' Expr								              {$$ = doLT($1, $3);};
BExpr		      :	Expr LTE Expr								              {$$ = doLTE($1, $3);};
BExpr		      :	Expr '>' Expr								              {$$ = doGT($1, $3);};
BExpr		      :	Expr GTE Expr								              {$$ = doGTE($1, $3);};
Idents        : Idents ',' Id                             {$$ = printNoNewLineComma($1, $3);};
Idents        : Id                                        {$$ = printNoNewLine($1);};
Exprs         : Exprs ',' Expr                            {$$ = printNoNewLineCommaInt($1, $3);};
Exprs         : Expr                                      {$$ = printNoNewLineInt($1);};
Expr			    :	Expr '+' Term								              {$$ = doAdd($1, $3); } ;
Expr			    :	Expr '-' Term								              {$$ = doSub($1, $3); } ;
Expr			    :	Term									                    {$$ = $1; } ;
Term		      :	Term '*' Factor								            {$$ = doMult($1, $3); } ;
Term		      :	Term '/' Factor								            {$$ = doDiv($1, $3); } ;
Term		      :	Term '%' Factor								            {$$ = doMod($1, $3); } ;
Term	 	      :	Factor									                  {$$ = $1; } ;
Factor        : XFactor EXPNT Factor                      {$$ = doExp($1, $3);};
Factor	 	    :	XFactor			  			                      {$$ = $1; } ;
XFactor       : '-' XFactor                               {$$ = doNegate($2);};
XFactor     	:	IntLit								                    {$$ = doIntLit(yytext); };
XFactor       :	Ident									                    {$$ = doRval(yytext); };
Id			      : Ident									                    {$$ = strdup(yytext);}

%%

int yyerror(char *s)  {
  writeIndicator(getCurrentColumnNum());
  writeMessage("Illegal Character in YACC");
  return 1;
}
