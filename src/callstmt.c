#include <string.h>
#include <assert.h>
#include "str.h"
#include "callstmt.h"
#include "locals.h"

//region forward declarations

void csBlock(Sexp* block);

//endregion

/**
 * Transforms expr-calls (calls that do not return void) to be statements.
 *
 * Is a no-op on calls that return void.
 *
 * @param call - the call that is transformed to be a statement
 */
void csCall(Sexp* block, Sexp* call) {
  if (strcmp(call->list[2]->value, "void") == 0) return;
  size_t ignored = newLocal();

  /* create a initializer for the ignored stack variable */
  Sexp* init = makeSexp(makeStr("$%lu", ignored), 0);

  /* create let from call */
  Sexp* let = makeSexp(copyStr("let"), 2);
  let->list[0] = init;
  let->list[1] = call;

  /* replace call with let */
  block->list[indexOfSexp(block, call)] = let;
}

void csStmt(Sexp* block, Sexp* s) {
  if (isDo(s)) {
    csBlock(s);
  }
  else if (isIf(s)) {
    csBlock(s->list[1]);
  }
  else if (isCallLike(s) && !isBecome(s)) { // call-like other than become
    assert(!isBecome(s));
    csCall(block, s);
  }
  else {
    /* statements without possibly tall expressions in them */
    int isOtherStatement = isAuto(s) || isLet(s) || isReturn(s) || isStore(s);
    if (!isOtherStatement) {
      printSexp(s);
    }
    assert(isOtherStatement);
  }
}

void csBlock(Sexp* block) {
  for (int i = 0; i < block->length; ++i) {
    csStmt(block, block->list[i]);
  }
}

void callStmt(Sexp* p) {
  for (int i = 0; i < p->length; ++i) {
    Sexp* child = p->list[i];
    if (isDef(child)) {
      initLocals(child);
      csBlock(child->list[3]);
    }
  }
}
