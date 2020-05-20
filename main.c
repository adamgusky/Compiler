#include <stdio.h>
#include "SymTab.h"
#include "IOMngr.h"

extern int yyparse();

SymTab *table;
SymTab *fList; // this is where I am going to store functions. This is so the last syscall of the program is still in main
SymTab *parameters;
int inAFunction;
int offset;
char * currentFunction; // the name of the function we're currently in
int stackPointerCounter;
//SymTab *ProcSymTab;
//struct SymEntry *entry;
//int inProc =0;
FILE *aFile;

int main(int argc, char * argv[]) {
	table = createSymTab(100);
	fList = createSymTab(50);
	parameters = createSymTab(50);
	offset = 0;
	inAFunction = 0;
	currentFunction = (char*)malloc(sizeof(char) * 20);
	stackPointerCounter = 4;
	//assumes there is a listing file
	openFiles(argv[1], argv[2]);
	if (argc == 4) {
		aFile = fopen(argv[3], "w");
	} else {
		aFile = stdout;
	}
	yyparse();
}
