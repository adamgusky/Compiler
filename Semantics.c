/* Semantics.c
   Support and semantic action routines.

*/

#include <strings.h>
#include <stdlib.h>

#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOMngr.h"

extern SymTab *table;

/* Semantics support routines */


struct ExprRes *  doIntLit(char * digits)  {

   struct ExprRes *res;

  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"li",TmpRegName(res->Reg),digits,NULL);

  return res;
}

struct ExprRes *  doRval(char * name)  {

   struct ExprRes *res;

   if (!findName(table, name)) {
		writeIndicator(getCurrentColumnNum());
		writeMessage("Undeclared variable");
   }
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"lw",TmpRegName(res->Reg),name,NULL);

  return res;
}

struct ExprRes *  doAdd(struct ExprRes * Res1, struct ExprRes * Res2)  {

   int reg;

  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doSub(struct ExprRes * Res1, struct ExprRes * Res2)  {

   int reg;

  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sub",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doNegate(struct ExprRes * Res1)  {
  struct ExprRes* Res2 = doIntLit("-1");
  int reg;

  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doMult(struct ExprRes * Res1, struct ExprRes * Res2)  {

   int reg;

  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doDiv(struct ExprRes * Res1, struct ExprRes * Res2)  {

   int reg;

  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"div",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doMod(struct ExprRes * Res1, struct ExprRes * Res2)  {
  int divideReg = AvailTmpReg();

  AppendSeq(Res1->Instrs, Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"div",
                                       TmpRegName(divideReg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));

  int multReg = AvailTmpReg();

  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
                                       TmpRegName(multReg),
                                       TmpRegName(divideReg),
                                       TmpRegName(Res2->Reg)));

   AppendSeq(Res1->Instrs,GenInstr(NULL,"sub",
                                        TmpRegName(Res1->Reg),
                                        TmpRegName(Res1->Reg),
                                        TmpRegName(multReg)));
   ReleaseTmpReg(divideReg);
   ReleaseTmpReg(multReg);
   free(Res2);
   return Res1;
}

extern struct ExprRes *  doExp(struct ExprRes * Res1, struct ExprRes * Res2) {
  int baseReg = Res1->Reg;
  int exponentReg = Res2->Reg;
  int counter = AvailTmpReg();
  int answer = AvailTmpReg();

  char* loop = GenLabel();
  char* exit = GenLabel();

  AppendSeq(Res1->Instrs, Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"addi",
                                       TmpRegName(answer),
                                       "$zero",
                                       "1"));

   AppendSeq(Res1->Instrs,GenInstr(NULL,"addi",
                                        TmpRegName(counter),
                                        "$zero",
                                        "0"));

   AppendSeq(Res1->Instrs, GenInstr(loop, NULL, NULL, NULL, NULL));

   AppendSeq(Res1->Instrs,GenInstr(NULL,"beq",
                                       TmpRegName(counter),
                                       TmpRegName(exponentReg),
                                       exit));

   AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
                                      TmpRegName(answer),
                                      TmpRegName(baseReg),
                                      TmpRegName(answer)));

  AppendSeq(Res1->Instrs,GenInstr(NULL,"addi",
                                      TmpRegName(counter),
                                      TmpRegName(counter),
                                      "1"));

  AppendSeq(Res1->Instrs,GenInstr(NULL,"j",
                                      loop,
                                      NULL,
                                      NULL));


  AppendSeq(Res1->Instrs, GenInstr(exit, NULL, NULL, NULL, NULL));
  Res1->Reg = answer;

  ReleaseTmpReg(counter);
  ReleaseTmpReg(answer);
  ReleaseTmpReg(baseReg);
  ReleaseTmpReg(exponentReg);

  return Res1;
}

struct BExprRes * doNot(struct BExprRes * Res1) {
  char* notLabel = GenLabel();

  AppendSeq(Res1->Instrs, GenInstr(NULL, "j", notLabel, NULL, NULL));
  AppendSeq(Res1->Instrs, GenInstr(Res1->Label, NULL, NULL, NULL, NULL));

  Res1->Label = notLabel;

  return Res1;
}

struct BExprRes * doOr(struct BExprRes* Res1, struct BExprRes* Res2) {
  char* label = GenLabel();

  AppendSeq(Res1->Instrs, GenInstr(NULL, "j", label, NULL, NULL));
  AppendSeq(Res1->Instrs, GenInstr(Res1->Label, NULL, NULL, NULL, NULL));

  AppendSeq(Res1->Instrs, Res2->Instrs);
  AppendSeq(Res1->Instrs, GenInstr(label, NULL, NULL, NULL, NULL));

  Res1->Label = Res2->Label;

  free(Res2);

  return Res1;
}


struct InstrSeq * doPrint(struct ExprRes * Expr) {

  struct InstrSeq *code;

  code = Expr->Instrs;

    AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
    AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg), NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
    AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
   AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    ReleaseTmpReg(Expr->Reg);
    free(Expr);

  return code;
}

struct InstrSeq * doPrintSpaces(struct ExprRes * Expr) {
  int baseReg = Expr->Reg;
  int counter = AvailTmpReg();

  struct ExprRes* space = doIntLit(" ");

  char* loop = GenLabel();
  char* exit = GenLabel();
  AppendSeq(Expr->Instrs,GenInstr(NULL,"addi",
  TmpRegName(counter),
  "$zero",
  "0"));

  AppendSeq(Expr->Instrs, GenInstr(loop, NULL, NULL, NULL, NULL));

  AppendSeq(Expr->Instrs,GenInstr(NULL,"beq",
  TmpRegName(counter),
  TmpRegName(baseReg),
  exit));


  AppendSeq(Expr->Instrs,GenInstr(NULL,"li","$v0","11",NULL));
  AppendSeq(Expr->Instrs,GenInstr(NULL,"li","$a0", "32", NULL));
  AppendSeq(Expr->Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  AppendSeq(Expr->Instrs,GenInstr(NULL,"addi",
  TmpRegName(counter),
  TmpRegName(counter),
  "1"));

  AppendSeq(Expr->Instrs,GenInstr(NULL,"j",
  loop,
  NULL,
  NULL));


  AppendSeq(Expr->Instrs, GenInstr(exit, NULL, NULL, NULL, NULL));

  ReleaseTmpReg(counter);
  ReleaseTmpReg(baseReg);

  return Expr->Instrs;
}

struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) {

  struct InstrSeq *code;


   if (!findName(table, name)) {
		writeIndicator(getCurrentColumnNum());
		writeMessage("Undeclared variable");
   }

  code = Expr->Instrs;

  AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));

  ReleaseTmpReg(Expr->Reg);
  free(Expr);

  return code;
}

extern struct BExprRes * doAnd(struct BExprRes * Res1,  struct BExprRes * Res2) {
  char * label = GenLabel();

  AppendSeq(Res1->Instrs, GenInstr(NULL, "j", label, NULL, NULL));
  AppendSeq(Res1->Instrs, GenInstr(Res1->Label, "j", Res2->Label, NULL, NULL));
  AppendSeq(Res1->Instrs, GenInstr(label, NULL, NULL, NULL, NULL));
  AppendSeq(Res1->Instrs, Res2->Instrs);
  Res1->Label = Res2->Label;

  free(Res2);
  return Res1;
}

extern struct BExprRes * doBExpr(struct ExprRes * Res1,  struct ExprRes * Res2) {
	struct BExprRes * bRes;
	AppendSeq(Res1->Instrs, Res2->Instrs);
 	bRes = (struct BExprRes *) malloc(sizeof(struct BExprRes));
	bRes->Label = GenLabel();
	AppendSeq(Res1->Instrs, GenInstr(NULL, "bne", TmpRegName(Res1->Reg), TmpRegName(Res2->Reg), bRes->Label));
	bRes->Instrs = Res1->Instrs;
	ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
	free(Res1);
	free(Res2);
	return bRes;
}

extern struct BExprRes * doFalseBExpr(struct ExprRes * Res1,  struct ExprRes * Res2) {
	struct BExprRes * bRes;
	AppendSeq(Res1->Instrs, Res2->Instrs);
 	bRes = (struct BExprRes *) malloc(sizeof(struct BExprRes));
	bRes->Label = GenLabel();
	AppendSeq(Res1->Instrs, GenInstr(NULL, "beq", TmpRegName(Res1->Reg), TmpRegName(Res2->Reg), bRes->Label));
	bRes->Instrs = Res1->Instrs;
	ReleaseTmpReg(Res1->Reg);
  	ReleaseTmpReg(Res2->Reg);
	free(Res1);
	free(Res2);
	return bRes;
}

extern struct BExprRes * doLT(struct ExprRes * Res1,  struct ExprRes * Res2) {
  struct ExprRes* boolRes;
  struct ExprRes* constant = doIntLit("1");
  boolRes = (struct ExprRes*) malloc(sizeof(struct ExprRes));
  boolRes->Reg = AvailTmpReg();


  AppendSeq(Res1->Instrs, Res2->Instrs);
  AppendSeq(Res1->Instrs, GenInstr(NULL, "slt", TmpRegName(boolRes->Reg), TmpRegName(Res1->Reg), TmpRegName(Res2->Reg)));
  boolRes->Instrs = Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);

  return doBExpr(boolRes, constant);
}

extern struct BExprRes * doLTE(struct ExprRes * Res1,  struct ExprRes * Res2) {
  struct ExprRes* boolRes;
  struct ExprRes* constant = doIntLit("0");
  boolRes = (struct ExprRes*) malloc(sizeof(struct ExprRes));
  boolRes->Reg = AvailTmpReg();


  AppendSeq(Res1->Instrs, Res2->Instrs);
  AppendSeq(Res1->Instrs, GenInstr(NULL, "slt", TmpRegName(boolRes->Reg), TmpRegName(Res2->Reg), TmpRegName(Res1->Reg)));
  boolRes->Instrs = Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);

  return doBExpr(boolRes, constant);
}

extern struct BExprRes * doGT(struct ExprRes * Res1,  struct ExprRes * Res2) {
  struct ExprRes* boolRes;
  struct ExprRes* constant = doIntLit("1");
  boolRes = (struct ExprRes*) malloc(sizeof(struct ExprRes));
  boolRes->Reg = AvailTmpReg();


  AppendSeq(Res1->Instrs, Res2->Instrs);
  AppendSeq(Res1->Instrs, GenInstr(NULL, "slt", TmpRegName(boolRes->Reg), TmpRegName(Res2->Reg), TmpRegName(Res1->Reg)));
  boolRes->Instrs = Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);

  return doBExpr(boolRes, constant);
}

extern struct BExprRes * doGTE(struct ExprRes * Res1,  struct ExprRes * Res2) {
  struct ExprRes* boolRes;
  struct ExprRes* constant = doIntLit("0");
  boolRes = (struct ExprRes*) malloc(sizeof(struct ExprRes));
  boolRes->Reg = AvailTmpReg();


  AppendSeq(Res1->Instrs, Res2->Instrs);
  AppendSeq(Res1->Instrs, GenInstr(NULL, "slt", TmpRegName(boolRes->Reg), TmpRegName(Res1->Reg), TmpRegName(Res2->Reg)));
  boolRes->Instrs = Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);

  return doBExpr(boolRes, constant);
}

struct InstrSeq* doPrintLine() {
  struct InstrSeq* code;
  code = GenInstr(NULL, "li", "$v0", "4", NULL);
  AppendSeq(code, GenInstr(NULL, "la", "$a0", "_nl", NULL));
  AppendSeq(code, GenInstr(NULL, "syscall", NULL, NULL, NULL));
  return code;
}

extern struct InstrSeq * doIf(struct BExprRes * bRes, struct InstrSeq * seq) {
	struct InstrSeq * seq2;
	seq2 = AppendSeq(bRes->Instrs, seq);
	AppendSeq(seq2, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
	free(bRes);
	return seq2;
}

extern struct ExprRes * printNoNewLine(char * Expr) {
    printf("no new line\n");

    struct ExprRes* result;

    result = doRval(Expr);

    AppendSeq(result->Instrs,GenInstr(NULL,"li","$v0","1",NULL));
    AppendSeq(result->Instrs,GenInstr(NULL,"move","$a0",TmpRegName(result->Reg),NULL));
    AppendSeq(result->Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    ReleaseTmpReg(result->Reg);

    WriteSeq(result->Instrs);

    return result;

}

extern struct ExprRes * printNoNewLineComma(struct ExprRes * Res, char * Expr) {
    struct ExprRes* result;

    result = doRval(Expr);
    AppendSeq( Res->Instrs, result->Instrs);

    AppendSeq(Res->Instrs,GenInstr(NULL,"li","$v0","11",NULL));
    AppendSeq(Res->Instrs,GenInstr(NULL,"li","$a0","44",NULL));
    AppendSeq(Res->Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    AppendSeq(Res->Instrs,GenInstr(NULL,"li","$v0","1",NULL));
    AppendSeq(Res->Instrs,GenInstr(NULL,"move","$a0",TmpRegName(result->Reg),NULL));
    AppendSeq(Res->Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));



    ReleaseTmpReg(result->Reg);
    WriteSeq(result->Instrs);

    return Res;

}

struct InstrSeq * addLine(struct ExprRes * Res1) {
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li","$v0","4",NULL));
    AppendSeq(Res1->Instrs, GenInstr(NULL,"la","$a0","_nl",NULL));
    AppendSeq(Res1->Instrs, GenInstr(NULL,"syscall",NULL,NULL,NULL));
    WriteSeq(Res1->Instrs);

    return Res1->Instrs;
}

/*

extern struct InstrSeq * doIf(struct ExprRes *res1, struct ExprRes *res2, struct InstrSeq * seq) {
	struct InstrSeq *seq2;
	char * label;
	label = GenLabel();
	AppendSeq(res1->Instrs, res2->Instrs);
	AppendSeq(res1->Instrs, GenInstr(NULL, "bne", TmpRegName(res1->Reg), TmpRegName(res2->Reg), label));
	seq2 = AppendSeq(res1->Instrs, seq);
	AppendSeq(seq2, GenInstr(label, NULL, NULL, NULL, NULL));
	ReleaseTmpReg(res1->Reg);
  	ReleaseTmpReg(res2->Reg);
	free(res1);
	free(res2);
	return seq2;
}

*/
void
Finish(struct InstrSeq *Code)
{ struct InstrSeq *code;
  //struct SymEntry *entry;
    int hasMore;
  struct Attr * attr;


  code = GenInstr(NULL,".text",NULL,NULL,NULL);
  //AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
  AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
  AppendSeq(code,Code);
  AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL));
  AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","4",NULL,NULL));
  AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));

 hasMore = startIterator(table);
 while (hasMore) {
	AppendSeq(code,GenInstr((char *) getCurrentName(table),".word","0",NULL,NULL));
    hasMore = nextEntry(table);
 }

  WriteSeq(code);

  return;
}
