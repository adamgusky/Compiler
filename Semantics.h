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




/* Semantics Actions */
extern struct ExprRes *  doIntLit(char * digits);
extern struct ExprRes *  doRval(char * name);
extern struct InstrSeq *  doAssign(char * name,  struct ExprRes * Res1);
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

extern void	Finish(struct InstrSeq *Code);
