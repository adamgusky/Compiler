yacc -d ExprEval.y
lex lex1.l
gcc lex.yy.c y.tab.c main.c semantics.c IOMngr.c SymTab.c CodeGen.c
