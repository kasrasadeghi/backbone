#include <string.h>
#include <assert.h>
#include "become.h"
#include "str.h"

/**
 * (become 'name 'types 'return-type 'args)
 *
 * if 'return-type == void
 *  => (call-tail 'name 'types 'return-type 'args)
 *     (return void)
 *
 * otherwise
 *     (return (call-tail 'name 'types 'return-type 'args) 'return-type)
 *
 */
void becomeBecome(Sexp* block, Sexp* call_tail) {
  /* make call-tail sexp */
  Sexp* return_type = call_tail->list[2];
  replaceValue(call_tail, copyStr("call-tail"));

  if (strcmp(return_type->value, "void") == 0) {
    /* (call-tail 'name 'types 'return-type 'args) *
     * (return void)                               */

    /* make return void */
    Sexp* return_sexp = makeSexp(copyStr("return"), 1);
    return_sexp->list[0] = makeSexp(copyStr("void"), 0);

    /* insert return_sexp after call_tail */
    insertSexp(block, return_sexp, indexOfSexp(block, call_tail) + 1);

  } else {
    /* (return (call-tail 'name 'types 'return-type 'args) 'return-type) */

    Sexp* return_sexp = makeSexp(copyStr("return"), 2);
    return_sexp->list[0] = call_tail;
    return_sexp->list[1] = makeSexp(copyStr(return_type->value), 0);

    /* replace call_tail with return_sexp in block */
    block->list[indexOfSexp(block, call_tail)] = return_sexp;
  }
}

void becomeStmt(Sexp* block, Sexp* s);

void becomeDo(Sexp* block) {
  for (int i = 0; i < block->length; ++i) {
    becomeStmt(block, block->list[i]);
  }
}

void becomeStmt(Sexp* block, Sexp* s) {
  if (isBecome(s)) {
    becomeBecome(block, s);
  } else if (isDo(s)) {
    becomeDo(s);
  } else if (isIf(s)) {
    becomeDo(s->list[1]);
  } else {
    /* other statements */
    int isOtherStatement = isAuto(s) || isLet(s) || isReturn(s) || isCall(s) || isCallVargs(s) || isCallTail(s) || isStore(s);
    if (!isOtherStatement) {
      printSexp(s);
    }
    assert(isOtherStatement);
  }
}

void becomeBlock(Sexp* block) {
  for (int i = 0; i < block->length; ++i) {
    becomeStmt(block, block->list[i]);
  }
}

void becomeDef(Sexp* s) {
  becomeBlock(s->list[3]);
}

void become(Sexp* p) {
  for (int i = 0; i < p->length; ++i) {
    Sexp* child = p->list[i];
    if (strcmp(child->value, "def") == 0) {
      becomeDef(child);
    }
  }
}
