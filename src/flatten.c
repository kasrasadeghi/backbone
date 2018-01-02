#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "flatten.h"
#include "str.h"

/**
 * returns true for expressions that are not values.
 */
int isTall(Sexp* s) {
  return isCall(s)
         || isCallVargs(s)
         || isCallTail(s)
         || isAdd(s)
         || isIcmp(s)
         || isLoad(s)
         || isIndex(s)
         || isCast(s)
      ;
}

static Sexp* _block = NULL;
static Sexp* _stmt = NULL;
static size_t _stack_counter = 0;

/**
 * Inserts a statement into the current definition, _def.
 *
 *  - Globals: _block
 *
 * Note: relies on the lack of nested definitions.
 *
 * @param stmt  - the statement to insert.
 * @param csi - the index to insert it at.
 */
static void insertStmt(Sexp* block, Sexp* stmt, int csi) {
  incrementLength(block);

  /* move everything from [csi, length) over, starting from the end */
  for (size_t si = block->length - 1; si >= csi; --si) {
    block->list[si] = block->list[si - 1];
  }

  /* place statement in block */
  block->list[csi] = stmt;
}

/**
 * Extracts an expression from a Sexp s at index index.
 *
 *  - Replaces the expression with a reference to a new local.
 *  - Creates a let expression that initializes the local, setting it to the extracted sexp.
 *
 * Globals:
 *  - _stack_counter++
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
 * Get the index of the current statement.
 *
 * @returns the index of the current statement in the current block.
 */
int currStmtIndex(Sexp* block, Sexp* stmt) {
  int csi = 0;
  for (; csi < block->length; ++csi) { // curr statement index = csi
    if (block->list[csi] == stmt) break;
  }
  if (csi == block->length) {
    fprintf(stderr, "backbone: could not find current statement in definition");
    exit(1);
  }

  return csi;
}

//region Forward Declarations

void fLet(Sexp* let);

//endregion

/**
 * Checks if s contains an expression at i, where it expects to be a value. If it does, the it flattens it.
 *
 *
 *
 * @param s
 * @param i
 */
void fTall(Sexp* s, int i) {
  if (isTall(s->list[i])) {
    Sexp* let = extractLet(s, i);
    int csi = currStmtIndex(_block, _stmt);
    insertStmt(_block, let, csi);

    /* recurse on the let we just inserted */
    Sexp* stmt_cache = _stmt;
    _stmt = let;
    fLet(let);
    _stmt = stmt_cache;
  }
}

void fCall(Sexp* call) {
  Sexp* args = call->list[3];
  for (int ai = 0; ai < args->length; ++ai) { // argument index = ai
    fTall(args, ai);
  }
}

/**
 * tall expression = expression that contains another expression
 * this should flatten every tall expression
 */
void fLet(Sexp* let) {
  Sexp* expr = let->list[1];
  if (isCall(expr) || isCallVargs(expr) || isCallTail(expr)) {
    fCall(expr);
    return;
  }
  else if (isAdd(expr)) {
    fTall(expr, 1);
    fTall(expr, 2);
    return;
  }
  else if (isLoad(expr)) {
    fTall(expr, 1);
    return;
  }
  else if (isIndex(expr)) {
    fTall(expr, 0);
    fTall(expr, 2);
    return;
  }
  else if (isCast(expr)) {
    fTall(expr, 2);
    return;
  }
  else if (isIcmp(expr)) {
    fTall(expr, 1);
    fTall(expr, 2);
    return;
  }
  assert(0);
}

/**
 * Globals:
 *  - _block
 *  - _stmt
 *
 * Transforms expr-calls (calls that do not return void) to be statements.
 *
 * Is a no-op on calls that return void.
 *
 * @param call - the call that is transformed to be a statement
 */
void callStmt(Sexp* block, Sexp* call) {
  if (strcmp(call->list[2]->value, "void") == 0) {
    fCall(call);
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
  block->list[currStmtIndex(block, call)] = let;

  /* flatten let */
  _stmt = let;
  fLet(let);
  _stmt = call;
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
void fBecome(Sexp* s) {
  /* make call-tail sexp */
  Sexp* return_type = s->list[2];
  free(s->value);
  s->value = copyStr("call-tail");

  if (strcmp(return_type->value, "void") == 0) {
    /* make return void */
    Sexp* return_sexp = makeSexp(copyStr("return"), 1);
    return_sexp->list[0] = makeSexp(copyStr("void"), 0);

    insertStmt(_block, return_sexp, currStmtIndex(_block, _stmt) + 1);

    /* flatten the call, the return is void */
    fCall(s);
  } else {
    Sexp* return_sexp = makeSexp(copyStr("return"), 2);
    return_sexp->list[0] = s;
    return_sexp->list[1] = sexp(copyStr(return_type->value));

    _block->list[currStmtIndex(_block, _stmt)] = return_sexp;
    _stmt = return_sexp;

    /* flatten the return, it has the call in it */
    fTall(return_sexp, 0);
  }
}

void fBlock(Sexp* block, int startIndex);

void fStmt(Sexp* s) {
  _stmt = s;

  if (isLet(s)) {
    fLet(s);
  }
  else if (isReturn(s)) {
    if (strcmp(s->list[0]->value, "void") != 0) {
      fTall(s, 0);
    }
  }
  else if (isIf(s)) {
    fTall(s, 0);
    fBlock(s, 1);
  }
  else if (isCall(s) || isCallVargs(s) || isCallTail(s)) {
    callStmt(_block, s);
  }
  else if (isStore(s)) {
    /* (store Value Type Ptr) */
    fTall(s, 0);
    fTall(s, 2);
  }
  else if (isBecome(s)) {
    fBecome(s);
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
  Sexp* block_cache = _block;
  _block = block;

  for (int i = startIndex; i < _block->length; ++i) {
    fStmt(_block->list[i]);
  }
  _block = block_cache;
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
