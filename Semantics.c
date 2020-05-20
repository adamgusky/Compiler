/* Semantics.c
   Support and semantic action routines.

*/

#include <strings.h>
#include <stdlib.h>
#include <string.h>

#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOMngr.h"

extern SymTab *table;
extern SymTab *fList; // a list of functions
extern SymTab *parameters;
extern int inAFunction;
extern int stackPointerCounter;
extern char * currentFunction;
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
   res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
   res->Reg = AvailTmpReg();
   printf("method called.\n");
   if (inAFunction) {
     printf("yes we are in a function\n");
     if (!findName(fList, currentFunction) && !findName(table, name)) {
       writeIndicator(getCurrentColumnNum());
       writeMessage("Undeclared Variable");
     } else {
       if (findName(fList, currentFunction)) {
         struct paramInfo* info = (struct paramInfo*)getCurrentAttr(fList);
         SymTab* vars = (SymTab*)info->vars;
         if (!findName(vars, name)) {
           if (!findName(table, name)) {
             writeIndicator(getCurrentColumnNum());
             writeMessage("Undeclared Variable");
           } else {
             res->Instrs = GenInstr(NULL, "lw", TmpRegName(res->Reg), name, NULL);
           }
         } else {
           char * stackPointer = getCurrentAttr(vars);
           printf("getting to getCurrentAttr(): %s", getCurrentAttr(vars));
           res->Instrs = GenInstr(NULL, "lw", TmpRegName(res->Reg), stackPointer, NULL);
         }
       }
     }
   } else {
       if (!findName(table, name)) {
         writeIndicator(getCurrentColumnNum());
         writeMessage("Undeclared Variable");
       } else {
         res->Instrs = GenInstr(NULL, "lw", TmpRegName(res->Reg), name, NULL);
       }
   }
  return res;
}

extern struct ExprRes * doArray (char * arrName, struct ExprRes* index) {
  struct ExprRes *rtrn;
  int address= AvailTmpReg();
  char* formattedAddress = (char*)malloc(sizeof(char)*10);

  rtrn = (struct ExprRes*)malloc(sizeof(struct ExprRes));
  rtrn->Reg = AvailTmpReg();
  rtrn->Instrs = index->Instrs;

  if (!findName(table, arrName)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared Variable");
  }


  AppendSeq(rtrn->Instrs, GenInstr(NULL, "la", TmpRegName(address), arrName, NULL));
  AppendSeq(rtrn->Instrs, GenInstr(NULL, "sll", TmpRegName(index->Reg), TmpRegName(index->Reg), "2"));
  AppendSeq(rtrn->Instrs, GenInstr(NULL, "add", TmpRegName(address), TmpRegName(address), TmpRegName(index->Reg)));
  sprintf(formattedAddress, "0(%s)", TmpRegName(address));
  AppendSeq(rtrn->Instrs, GenInstr(NULL, "lw", TmpRegName(rtrn->Reg), formattedAddress, NULL));

  ReleaseTmpReg(index->Reg);
  ReleaseTmpReg(address);

  free(index);
  free(formattedAddress);

  return rtrn;

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
  printf("Reg1: %s\nReg2: %s\n", TmpRegName(Res1->Reg), TmpRegName(Res2->Reg));

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

  code = Expr->Instrs;

  if (inAFunction) {
    if (!findName(fList, currentFunction) && !findName(table, name)) {
      writeIndicator(getCurrentColumnNum());
      writeMessage("Undeclared Variable");
    } else {
      if (findName(fList, currentFunction)) {
        struct paramInfo* info = (struct paramInfo*)getCurrentAttr(fList);
        SymTab * vars = (SymTab*)info->vars;
        if (!findName(vars, name)) {
          if (!findName(table, name)) {
            writeIndicator(getCurrentColumnNum());
            writeMessage("Undeclared Variable");
          } else {
            AppendSeq(code, GenInstr(NULL, "sw", TmpRegName(Expr->Reg), name, NULL));
          }
        } else {
          char * stackPointer = getCurrentAttr(vars);
          AppendSeq(code, GenInstr(NULL, "sw", TmpRegName(Expr->Reg), stackPointer, NULL));
        }
      }
    }
  } else {
      if (!findName(table, name)) {
        writeIndicator(getCurrentColumnNum());
        writeMessage("Undeclared Variable");
      } else {
        AppendSeq(code, GenInstr(NULL, "sw", TmpRegName(Expr->Reg), name, NULL));
      }
  }


  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  return code;
}

extern struct InstrSeq * assignArrIndex(char * name, struct ExprRes * byteIndex, struct ExprRes * expr) {
  struct InstrSeq *code;
  int address = AvailTmpReg();
  int specAddress = AvailTmpReg();
  char* specAddressStr = (char*)malloc(sizeof(char)*20);

  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }

  code = AppendSeq(byteIndex->Instrs, expr->Instrs);
  AppendSeq(code, GenInstr(NULL, "la", TmpRegName(address), name, NULL));
  AppendSeq(code, GenInstr(NULL, "add", TmpRegName(byteIndex->Reg), TmpRegName(byteIndex->Reg), TmpRegName(byteIndex->Reg)));
  AppendSeq(code, GenInstr(NULL, "add", TmpRegName(byteIndex->Reg), TmpRegName(byteIndex->Reg), TmpRegName(byteIndex->Reg)));
  AppendSeq(code, GenInstr(NULL, "add", TmpRegName(specAddress), TmpRegName(byteIndex->Reg), TmpRegName(address)));

  sprintf(specAddressStr, "0(%s)", TmpRegName(specAddress));
  AppendSeq(code, GenInstr(NULL, "sw", TmpRegName(expr->Reg), specAddressStr, NULL));

  ReleaseTmpReg(byteIndex->Reg);
  ReleaseTmpReg(expr->Reg);
  ReleaseTmpReg(address);
  ReleaseTmpReg(specAddress);

  free(byteIndex);
  free(expr);

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

extern struct ExprRes * printNoNewLineInt(struct ExprRes * result) {


  AppendSeq(result->Instrs,GenInstr(NULL,"li","$v0","1",NULL));
  AppendSeq(result->Instrs,GenInstr(NULL,"move","$a0",TmpRegName(result->Reg),NULL));
  AppendSeq(result->Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  ReleaseTmpReg(result->Reg);
  return result;
}

extern struct ExprRes * printNoNewLineCommaInt(struct ExprRes * Res, struct ExprRes * result ) {
    AppendSeq( Res->Instrs, result->Instrs);

    AppendSeq(Res->Instrs,GenInstr(NULL,"li","$v0","11",NULL));
    AppendSeq(Res->Instrs,GenInstr(NULL,"li","$a0","44",NULL));
    AppendSeq(Res->Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    AppendSeq(Res->Instrs,GenInstr(NULL,"li","$v0","1",NULL));
    AppendSeq(Res->Instrs,GenInstr(NULL,"move","$a0",TmpRegName(result->Reg),NULL));
    AppendSeq(Res->Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));



    ReleaseTmpReg(result->Reg);

    return Res;

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
extern struct InstrSeq * doIf(struct BExprRes * bRes, struct InstrSeq * seq) {
	struct InstrSeq * seq2;
	seq2 = AppendSeq(bRes->Instrs, seq);
	AppendSeq(seq2, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
	free(bRes);
	return seq2;
}

extern struct InstrSeq * doIfElse(struct BExprRes * bRes, struct InstrSeq * seq1, struct InstrSeq * seq2) {
	struct InstrSeq * seq;
	seq = AppendSeq(bRes->Instrs, seq1);
  char* endLabel = GenLabel();

  AppendSeq(seq, GenInstr(NULL, "j", endLabel, NULL, NULL));
  AppendSeq(seq, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
  AppendSeq(seq, seq2);
  AppendSeq(seq, GenInstr(endLabel, NULL, NULL, NULL, NULL));

	free(bRes);
	return seq;
}

extern struct InstrSeq * doWhile(struct BExprRes * bRes, struct InstrSeq * seq) {
	struct InstrSeq * rtrn;
  char* start = GenLabel();


  rtrn = AppendSeq(GenInstr(start, NULL, NULL, NULL, NULL), bRes->Instrs);
  AppendSeq(rtrn, seq);
	AppendSeq(rtrn, GenInstr(NULL, "j", start, NULL, NULL));
  AppendSeq(rtrn, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));

	free(bRes);
	return rtrn;
}

extern struct InstrSeq * doFor(char * charVar1, struct ExprRes * res1, struct BExprRes * bRes1, char * var2, struct ExprRes * res2, struct InstrSeq * seq) {
	struct InstrSeq * rtrn;
  char* start = GenLabel();

  struct ExprRes * var1 = doRval(charVar1);
  rtrn = AppendSeq(doAssign(charVar1, res1), GenInstr(start, NULL, NULL, NULL, NULL));
  AppendSeq(rtrn, bRes1->Instrs); // add start label before instructions

  AppendSeq(rtrn, seq); //
  AppendSeq(rtrn, doAssign(var2, res2));
	AppendSeq(rtrn, GenInstr(NULL, "j", start, NULL, NULL)); // jump back to if statement to see if you loop again
  AppendSeq(rtrn, GenInstr(bRes1->Label, NULL, NULL, NULL, NULL)); // append the end label

	free(bRes1);
	return rtrn;
}


extern void doNoParams(char * type, char * name, struct InstrSeq * seq) {
  struct InstrSeq * rtrn;
  printf("in donoparams\n");
  rtrn = GenInstr(name, NULL, NULL, NULL, NULL);
  AppendSeq(rtrn, SaveSeq());
  AppendSeq(rtrn, GenInstr(NULL, "addi", "$sp", "$sp", "-8"));
  AppendSeq(rtrn, GenInstr(NULL, "sw", "$ra", "($sp)", NULL));
  AppendSeq(rtrn, seq);
  AppendSeq(rtrn, GenInstr(NULL, "lw", "$ra", "($sp)", NULL));
  AppendSeq(rtrn, GenInstr(NULL, "addi", "$sp", "$sp", "8"));
  AppendSeq(rtrn, RestoreSeq());
  AppendSeq(rtrn, GenInstr(NULL, "jr", "$ra", NULL, NULL));

  printf("\n\n %d \n\n", findName(fList, name));

  printf("current function %s \n", currentFunction);
  if (findName(fList, name)) {
    struct paramInfo * params = (struct paramInfo*)getCurrentAttr(fList);
    params->instructions = rtrn;
  }
  inAFunction = 0;
}


extern struct ExprRes * doParamFuncAssign(char * func) {
  struct ExprRes * rtrn;
  inAFunction = 1;
  printf("InAFunction = %d\n", inAFunction);
  rtrn = (struct ExprRes*) malloc(sizeof(struct ExprRes));
  rtrn->Reg = AvailTmpReg();
  if (!findName(table, func)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared Variable");
  }

  rtrn->Instrs = callVoidNoParams(func);

  AppendSeq(rtrn->Instrs, GenInstr(NULL, "lw", TmpRegName(rtrn->Reg), "-4($sp)", NULL));

  // free
  return rtrn;
}

extern struct ExprRes * doFuncAssign(char * func) {
  struct ExprRes * rtrn;
  inAFunction = 1;
  printf("InAFunction = %d\n", inAFunction);
  rtrn = (struct ExprRes*) malloc(sizeof(struct ExprRes));
  rtrn->Reg = AvailTmpReg();

  if(findName(fList, func)){
    if(((struct paramInfo *)getCurrentAttr(fList))->type == "void"){
      writeIndicator(getCurrentColumnNum());
      writeMessage("Error on void");
    }
  }else{
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared Function");
  }

  inAFunction = 1;

  // if (inAFunction) {
  //   if (!findName(fList, currentFunction) && !findName(table, func)) {
  //     writeIndicator(getCurrentColumnNum());
  //     writeMessage("Undeclared Variable");
  //   } else {
  //     if (findName(fList, currentFunction)) {
  //       struct paramInfo* info = (struct paramInfo*)getCurrentAttr(fList);
  //       SymTab * vars = (SymTab*)info->vars;
  //       if (!findName(vars, func)) {
  //         if (!findName(table, func)) {
  //           writeIndicator(getCurrentColumnNum());
  //           writeMessage("Undeclared Variable");
  //         } else {
  //           AppendSeq(rtrn->Instrs, GenInstr(NULL, "sw", TmpRegName(rtrn->Reg), func, NULL));
  //         }
  //       } else {
  //         char * stackPointer = getCurrentAttr(vars);
  //         AppendSeq(rtrn->Instrs, GenInstr(NULL, "sw", TmpRegName(rtrn->Reg), stackPointer, NULL));
  //       }
  //     }
  //   }
  // } else {
  //     if (!findName(table, func)) {
  //       writeIndicator(getCurrentColumnNum());
  //       writeMessage("Undeclared Variable");
  //     } else {
  //       AppendSeq(rtrn->Instrs, GenInstr(NULL, "sw", TmpRegName(rtrn->Reg), func, NULL));
  //     }
  // }



  //
  // rtrn->Instrs = callVoidNoParams(func);
  //
  // AppendSeq(rtrn->Instrs, GenInstr(NULL, "lw", TmpRegName(rtrn->Reg), "-4($sp)", NULL));

  // free
  return rtrn;
}

int functionEnterName(char * name) {
  printf("Function enter name\n");
  char * stackPointerLocation = (char *)malloc(sizeof(char)*10);
  struct paramInfo* info;
  SymTab * vars;
  if (enterName(fList, currentFunction)) {
    printf("In if:\n");

    info = malloc(sizeof(struct paramInfo));
    vars = createSymTab(5);
    info->vars = (SymTab *)(vars);
    if (enterName(vars, name)) {
      sprintf(stackPointerLocation, "%d($sp)", stackPointerCounter);
      printf("Entering at: %s\n", stackPointerLocation);
      setCurrentAttr(vars, (void*)stackPointerLocation);
      stackPointerCounter = stackPointerCounter +4;
    }
    setCurrentAttr(fList, info);
  } else {
    printf("In else:\n");

    // already a function
    info = (struct paramInfo *)getCurrentAttr(fList);
    vars = (SymTab*)info->vars;
    if (enterName(vars, name)) {
      sprintf(stackPointerLocation, "%d($sp)", stackPointerCounter);
      printf("Entering at this location %s\n", stackPointerLocation);
      setCurrentAttr(vars, (void*)stackPointerLocation);
      stackPointerCounter = stackPointerCounter + 4;
    }
  }
  printf("finished functionEnterName()\n");

  return 1;
}




// needs to return ExprRes
extern struct ExprRes * addToStack(struct ExprRes * expr) {
  // printf("in addtostack\n");
  // /* Adds to the stack */
  //
  // if (enterName(fList, currentFunction)) {
  //   // use paramInfo and add a SymEntry with name and stack point to it's vars
  //   printf("current function: %s\n", currentFunction);
  //   struct paramInfo * thisFuncInfo = getCurrentAttr(fList);
  //   SymTab * thisFunc = thisFuncInfo->vars;
  //   findName(thisFunc, PARAMETER'S NAME);
  //   char * spLocation = enterName(thisFunc, getCurrentAttr(thisFunc));
  //
  //   AppendSeq(expr->Instrs, GenInstr(NULL, "subu", "$sp", "$sp", "4"));
  //   AppendSeq(expr->Instrs, GenInstr(NULL, "sw", TmpRegName(expr->Reg), spLocation, NULL));


  // }
  printf("the argument <%d> was added to: %s\n", expr->Reg, currentFunction);
  // printf(", we get %d\n\n", stack);
  // then create a function that pops the parameters
  return expr;
}



extern struct InstrSeq * callVoidNoParams(char * name) {
  struct InstrSeq * rtrn;

  rtrn = GenInstr(NULL, "jal", name, NULL, NULL);
  return rtrn;
}

extern struct InstrSeq * funcReturn(struct ExprRes * expr) {
  struct InstrSeq * rtrn;
  rtrn = expr->Instrs;
  AppendSeq(rtrn, GenInstr(NULL, "sw", TmpRegName(expr->Reg), "4($sp)", NULL));
  return rtrn;
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
  int hasMoreFunctions;
  struct Attr * attr;
  char * sizeString = (char *)malloc(sizeof(char) * 10);


  code = GenInstr(NULL,".text",NULL,NULL,NULL);
  //AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
  AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
  AppendSeq(code,Code);
  AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL));
  AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));

  hasMoreFunctions = startIterator(fList);
  while (hasMoreFunctions) {
    if (getCurrentAttr(fList)) {
      AppendSeq(code, ((struct paramInfo *)getCurrentAttr(fList))->instructions);
    }
    hasMoreFunctions = nextEntry(fList);
  }

  AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","4",NULL,NULL));
  // AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));



 hasMore = startIterator(table);

 while (hasMore) {
    if (getCurrentAttr(table)) {
      // there is a current attr

      int size = 4 * ((int) getCurrentAttr(table));

      sprintf(sizeString, "%d", size);
      AppendSeq(code, GenInstr((char *) getCurrentName(table), ".space", sizeString, NULL, NULL));

    } else {
      // else print 0 for it's attr
      AppendSeq(code,GenInstr((char *) getCurrentName(table),".word","0",NULL,NULL));
    }
    hasMore = nextEntry(table);
 }
 // had to move this under the .space, i don't know why yet but it does work again
 AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));


  WriteSeq(code);

  return;
}
