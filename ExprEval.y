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
extern SymTab *parameters;
extern SymTab *fList;
extern int inAFunction;
extern char *currentFunction;
extern int stackPointCounter;

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
%type <ExprRes> ExprsParams
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
%token ELSE
%token EQ
%token NOTEQ
%token LTE
%token GTE
%token AMP
%token OR
%token EXPNT
%token PrintLine
%token PrintSpaces
%token WHILE
%token FOR
%token VOID
%token RETURN


%%


Prog			    :	Declarations StmtSeq						          {Finish($2); } ;
Declarations	:	Dec Declarations							            {};
Declarations	:											                      {};

Dec			      :	Int Id {inAFunction == 1 ? functionEnterName($2) : enterName(table, $2); }';'	      {};
Dec           : Int Id {enterName(table, $2);} '[' IntLit {setCurrentAttr(table, (void*)atoi(yytext)); }  ']' ';' {};
Dec           : VOID Id '(' ')' {inAFunction = 1; currentFunction = $2;} '{' StmtSeq '}'           { doNoParams("void", $2, $7);};
Dec           : VOID Id '(' {inAFunction = 1; currentFunction = $2;} Params ')' '{' StmtSeq '}'    { doNoParams("void", $2, $8);};
Dec           : Int Id '(' ')' {inAFunction = 1;currentFunction = $2;} '{' StmtSeq '}'            { doNoParams("int", $2, $7);};

StmtSeq       :	Stmt StmtSeq								              {$$ = AppendSeq($1, $2); } ;
StmtSeq		    :											                      {$$ = NULL;} ;
Stmt          : Dec                                       {}
Stmt			    :	Write Expr ';'								            {$$ = doPrint($2); printf("well its called and the input is <%s>\n", $2); };
Stmt          : Write '(' Exprs ')' ';'                   {$$ = addLine($3);};
Stmt          : Read '(' Idents ')' ';'                   {$$ = addLine($3); };
Stmt          : PrintLine ';'                             {$$ = doPrintLine();};
Stmt			    :	Id '=' Expr ';'								            {$$ = doAssign($1, $3);} ;
Stmt          : Id '[' Expr ']' '=' Expr ';'              {$$ = assignArrIndex($1, $3, $6);};
Stmt			    :	IF '(' BStmt ')' '{' StmtSeq '}'	        {$$ = doIf($3, $6);};
Stmt			    :	IF '(' BStmt ')' '{' StmtSeq '}' ELSE '{' StmtSeq '}'	                      {$$ = doIfElse($3, $6, $10);};
Stmt          : PrintSpaces '(' Expr ')'  ';'             {$$ = doPrintSpaces($3);};
Stmt          : WHILE '(' BStmt ')' '{' StmtSeq'}'        {$$ = doWhile($3, $6);};
Stmt          : FOR '(' Id '=' Expr ';' BStmt ';' Id '=' Expr ')' '{' StmtSeq '}'           {$$ = doFor($3, $5, $7, $9, $11, $14);};
Stmt          : Id '(' ')' ';'                            {$$ = callVoidNoParams($1);};
Stmt			    :	Id '(' ExprsParams ')' ';'				              {$$ = callVoidNoParams($1);};
Stmt          : RETURN Expr ';'                           {$$ = funcReturn($2);};
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
Params        : Params ',' Int Id {functionEnterName($4);} {}
Params        : Int Id {functionEnterName($2);}            {}
Idents        : Idents ',' Id                             {$$ = printNoNewLineComma($1, $3);};
Idents        : Id                                        {$$ = printNoNewLine($1);};
Exprs         : Exprs ',' Expr                            {$$ = printNoNewLineCommaInt($1, $3);};
Exprs         : Expr                                      {$$ = printNoNewLineInt($1);};
ExprsParams   : ExprsParams ',' Expr                      {$$ = addToStack($3);};
ExprsParams   : Expr                                      {$$ = addToStack($1);}; // keep track of funcName
Expr			    :	Expr '+' Term								              {$$ = doAdd($1, $3); } ;
Expr			    :	Expr '-' Term								              {$$ = doSub($1, $3); } ;
Expr			    :	Term									                    {$$ = $1; } ;
Expr			    :	Id '(' ')'						                    {$$ = doFuncAssign($1); } ;
Expr			    :	Id '(' ExprsParams ')'                    {$$ = doFuncAssign($1);} ;
Term		      :	Term '*' Factor								            {$$ = doMult($1, $3); } ;
Term		      :	Term '/' Factor								            {$$ = doDiv($1, $3); } ;
Term		      :	Term '%' Factor								            {$$ = doMod($1, $3); } ;
Term	 	      :	Factor									                  {$$ = $1; } ;
Factor        : XFactor EXPNT Factor                      {$$ = doExp($1, $3);};
Factor	 	    :	XFactor			  			                      {$$ = $1; } ;
XFactor       : '-' XFactor                               {$$ = doNegate($2);};
XFactor       : '(' Expr ')'                              {$$ = $2;};
XFactor     	:	IntLit								                    {$$ = doIntLit(yytext); };
XFactor       :	Id									                      {$$ = doRval($1); printf("doRval is intaking: %s", $1);};
XFactor       : Id '[' Expr ']'                           {$$ = doArray($1, $3);};
Id			      : Ident									                    {$$ = strdup(yytext);}

%%

int yyerror(char *s)  {
  writeIndicator(getCurrentColumnNum());
  writeMessage("Illegal Character in YACC");
  return 1;
}
