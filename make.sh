lex lex1.l
yacc -d ExprEval.y
gcc lex.yy.c y.tab.c main.c semantics.c IOMngr.c SymTab.c CodeGen.c
