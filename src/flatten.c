#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "flatten.h"
#include "str.h"

static size_t _stack_counter = 0;

//region Forward Declarations

void fBlock(Sexp* block, int startIndex);
void fLet(Sexp* block, Sexp* let);

//endregion

/**
 * Extracts an expression from a Sexp s at index index.
 *
 *  - Replaces the expression with a reference to a new local.
 *  - Creates a let expression that initializes the local, setting it to the extracted sexp.
 *
 * Globals:
 *  - _stack_counter
 *
 * @param s
 * @param index
 * @return the let expression to be inserted into the current definition, before the current stmt.
 */
Sexp* extractLet(Sexp* const s, const int index) {
  Sexp* expr = s->list[index];
  size_t local = _stack_counter++;

  /* replace with reference to local */
  Sexp* ref = makeSexp(makeStr("$%lu", local), 0);
  s->list[index] = ref;

  /* make an initializer of the local */
  Sexp* init = makeSexp(copyStr(ref->value), 0);

  /* create a let to insert into the definition */
  Sexp* let = makeSexp(copyStr("let"), 2);
  let->list[0] = init;
  let->list[1] = expr;

  return let;
}

/**
 * Checks if s contains an expression at i, where it expects to be a value. If it does, the it flattens it.
 *
 * Takes a block, a statement in the block, an s-expression in the block, and an index in that s-expression.
 *
 * If flattening proceeds, the expression at s[i] is extracted into a let before stmt in the block.
 *
 * Note: parent will equal stmt when the s-expression to flatten is in a statement.
 */
void fTall(Sexp* block, Sexp* stmt, Sexp* parent, int i) {
  if (isTall(parent->list[i])) {
    Sexp* let = extractLet(parent, i);
    size_t csi = indexOfSexp(block, stmt);
    insertSexp(block, let, csi);

    /* recurse on the let we just inserted */
    fLet(block, let);
  }
}

/**
 * Takes a block, a statement, and a call. Goes through the arguments of the call and flattens them.
 *
 * The arguments that actually needed flattening are placed in the block before stmt.
 */
void fCall(Sexp* block, Sexp* stmt, Sexp* call) {
  Sexp* args = call->list[3];
  for (int ai = 0; ai < args->length; ++ai) { // argument index = ai
    fTall(block, stmt, args, ai);
  }
}

/**
 * tall expression = expression that contains another expression
 * this should flatten every tall expression
 */
void fLet(Sexp* block, Sexp* let) {
  Sexp* expr = let->list[1];
  if (isCall(expr) || isCallVargs(expr) || isCallTail(expr)) {
    fCall(block, let, expr);
    return;
  }
  else if (isAdd(expr)) {
    fTall(block, let, expr, 1);
    fTall(block, let, expr, 2);
    return;
  }
  else if (isLoad(expr)) {
    fTall(block, let, expr, 1);
    return;
  }
  else if (isIndex(expr)) {
    fTall(block, let, expr, 0);
    fTall(block, let, expr, 2);
    return;
  }
  else if (isCast(expr)) {
    fTall(block, let, expr, 2);
    return;
  }
  else if (isIcmp(expr)) {
    fTall(block, let, expr, 0);
    fTall(block, let, expr, 1);
    return;
  }
  assert(0);
}

/**
 * Transforms expr-calls (calls that do not return void) to be statements.
 *
 * Is a no-op on calls that return void.
 *
 * @param call - the call that is transformed to be a statement
 */
void callStmt(Sexp* block, Sexp* call) {
  if (strcmp(call->list[2]->value, "void") == 0) {
    fCall(block, call, call);
    return;
  }

  size_t ignored = _stack_counter++;

  /* create a initializer for the ignored stack variable */
  Sexp* init = makeSexp(makeStr("$%lu", ignored), 0);

  /* create let from call */
  Sexp* let = makeSexp(copyStr("let"), 2);
  let->list[0] = init;
  let->list[1] = call;

  /* replace call with let */
  block->list[indexOfSexp(block, call)] = let;

  /* flatten let */
  fLet(block, let);
}

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
void fBecome(Sexp* block, Sexp* call_tail) {
  /* make call-tail sexp */
  Sexp* return_type = call_tail->list[2];
  free(call_tail->value);
  call_tail->value = copyStr("call-tail");

  if (strcmp(return_type->value, "void") == 0) {
    /* (call-tail 'name 'types 'return-type 'args)
     * (return void)
     */

    /* make return void */
    Sexp* return_sexp = makeSexp(copyStr("return"), 1);
    return_sexp->list[0] = makeSexp(copyStr("void"), 0);

    /* insert return_sexp after call_tail */
    insertSexp(block, return_sexp, indexOfSexp(block, call_tail) + 1);

    /* flatten the call. no need to flatten the return, it is void */
    fCall(block, call_tail, call_tail);
  } else {
    /* (return (call-tail 'name 'types 'return-type 'args) 'return-type)
     */

    Sexp* return_sexp = makeSexp(copyStr("return"), 2);
    return_sexp->list[0] = call_tail;
    return_sexp->list[1] = makeSexp(copyStr(return_type->value), 0);

    /* replace call_tail with return_sexp in block */
    block->list[indexOfSexp(block, call_tail)] = return_sexp;

    /* flatten the return, it has the call in it */
    fTall(block, return_sexp, return_sexp, 0);
  }
}



void fStmt(Sexp* block, Sexp* s) {

  if (isLet(s)) {
    fLet(block, s);
  }
  else if (isReturn(s)) {
    if (strcmp(s->list[0]->value, "void") != 0) {
      fTall(block, s, s, 0);
    }
  }
  else if (isIf(s)) {
    fTall(block, s, s, 0);
    fBlock(s, 1);
  }
  else if (isBecome(s)) {
    fBecome(block, s);
  }
  else if (isCallLike(s)) { // call-like other than become
    callStmt(block, s);
  }
  else if (isStore(s)) {
    /* (store Value Type Ptr) */
    fTall(block, s, s, 0);
    fTall(block, s, s, 2);
  }
  else {
    /* statements without possibly tall expressions in them */
    int isOtherStatement = isAuto(s);
    if (!isOtherStatement) {
      printSexp(s);
    }
    assert(isOtherStatement);
  }
}

void fBlock(Sexp* block, int startIndex) {
  for (int i = startIndex; i < block->length; ++i) {
    fStmt(block, block->list[i]);
  }
}

void fDef(Sexp* s) {
  _stack_counter = 0;
  fBlock(s, 3);
}

void flatten(Sexp* p) {
  for (int i = 0; i < p->length; ++i) {
    Sexp* child = p->list[i];
    if (strcmp(child->value, "def") == 0) {
      fDef(child);
    }
  }
}
