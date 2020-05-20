/* Semantics.h
   The action and supporting routines for performing semantics processing.
*/

/* Semantic Records */
struct IdList {
  struct SymEntry * TheEntry;
  struct IdList * Next;
};

struct ExprRes {
  int Reg;
  struct InstrSeq * Instrs;
};

struct ExprResList {
	struct ExprRes *Expr;
	struct ExprResList * Next;
};

struct BExprRes {
  char * Label;
  struct InstrSeq * Instrs;
};

struct paramInfo {
  char * type;
  struct SymTab * vars; // a SymTab to store the names of parameters and where they are located in the stack
  struct InstrSeq * instructions;
};




/* Semantics Actions */
extern struct ExprRes *  doIntLit(char * digits);
extern struct ExprRes *  doRval(char * name);
extern struct InstrSeq *  doAssign(char * name,  struct ExprRes * Res1);
extern struct InstrSeq * assignArrIndex(char * name, struct ExprRes * index, struct ExprRes * expr);
extern struct ExprRes * doAdd(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct BExprRes * doAnd(struct BExprRes * Res1,  struct BExprRes * Res2);
struct BExprRes * doOr(struct BExprRes* Res1, struct BExprRes* Res2);
extern struct ExprRes *  doSub(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes *  doNegate(struct ExprRes * Res1);
extern struct ExprRes *  doMult(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doDiv(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doExp(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct InstrSeq * addLine(struct ExprRes * Res1);
extern struct BExprRes * doNot(struct BExprRes * BRes);
extern struct ExprRes *  doMod(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct BExprRes * doLT(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct BExprRes * doLTE(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct BExprRes * doGT(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct BExprRes * doGTE(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct InstrSeq *  doPrint(struct ExprRes * Expr);
extern struct ExprRes * printNoNewLine(char * Expr);
extern struct ExprRes * printNoNewLineInt(struct ExprRes * result);
extern struct ExprRes * printNoNewLineComma(struct ExprRes * Res, char * Expr);
extern struct ExprRes * printNoNewLineCommaInt(struct ExprRes * Res, struct ExprRes * result );
extern struct InstrSeq * doPrintLine();
extern struct InstrSeq *  doPrintSpaces(struct ExprRes * Expr);
extern struct BExprRes * doBExpr (struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct BExprRes * doFalseBExpr(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct InstrSeq * doIf(struct BExprRes *bRes, struct InstrSeq * seq);
extern struct InstrSeq * doWhile(struct BExprRes *bRes, struct InstrSeq * seq);
extern struct InstrSeq * doFor(char * charVar1, struct ExprRes * res1, struct BExprRes * bRes1, char * var2, struct ExprRes * bRes2, struct InstrSeq * seq);
extern struct InstrSeq * doIfElse(struct BExprRes * bRes, struct InstrSeq * seq1, struct InstrSeq * seq2);
extern struct ExprRes * doArray (char * arrName, struct ExprRes* index);
extern void doNoParams(char * type, char * name, struct InstrSeq * seq);
extern struct InstrSeq * callVoidNoParams(char * name);
extern struct ExprRes * doFuncAssign(char * func);
extern struct InstrSeq * funcReturn(struct ExprRes * expr);
extern struct ExprRes * addToStack(struct ExprRes * expr);


extern int functionEnterName(char * name);


extern void	Finish(struct InstrSeq *Code);
