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
 *  - Increases the length of _def by one.
 *  - Globals: _defi, _def, _p, _block
 *
 * Note: relies on the lack of nested definitions.
 *
 * @param stmt  - the statement to insert.
 * @param csi - the index to insert it at.
 */
static void insertStmt(Sexp* stmt, int csi) {
  incrementLength(_block);

  /* move everything from [csi, length) over, starting from the end */
  for (size_t si = _block->length - 1; si >= csi; --si) {
    _block->list[si] = _block->list[si - 1];
  }

  /* place statement in block */
  _block->list[csi] = stmt;
}

/**
 * Extracts an expression from an Sexp s at index index.
 *
 *  - Replaces the expression with a reference to a new local.
 *  - Creates a let expression that initializes the local, setting it to the extracted sexp.
 *
 * @param s
 * @param index
 * @return the let expression to be inserted into the current definition, before the current stmt.
 */
Sexp* extractLet(Sexp* s, int index) {
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
 * @returns the index of the current statement _stmt in the current definition _def.
 */
int currStmtIndex() {
  int csi = 0;
  for (; csi < _block->length; ++csi) { // curr statement index = csi
    if (_block->list[csi] == _stmt) break;
  }
  if (csi == _block->length) {
    fprintf(stderr, "backbone: could not find current statement in definition");
    exit(1);
  }

  return csi;
}

//region Forward Declarations

void fLet(Sexp* let);

//endregion

void fTall(Sexp* s, int i) {
  if (isTall(s->list[i])) {
    Sexp* let = extractLet(s, i);
    int csi = currStmtIndex();
    insertStmt(let, csi);

    /* recurse on the let we just inserted */
    Sexp* stmt_cache = _stmt;
    _stmt = let;
    fLet(let);
    _stmt = stmt_cache;
  }
}

void fCall(Sexp* s) {
  Sexp* args = s->list[3];
  for (int ai = 0; ai < args->length; ++ai) { // argument index = ai
    fTall(args, ai);
  }
}

/**
 * tall expression = expression that contains another expression
 * this should flatten every tall expression
 */
void fLet(Sexp* let) {
  Sexp* s = let->list[1];
  if (isCall(s) || isCallVargs(s) || isCallTail(s)) {
    fCall(s);
    return;
  }
  else if (isAdd(s)) {
    fTall(s, 1);
    fTall(s, 2);
    return;
  }
  else if (isLoad(s)) {
    fTall(s, 1);
    return;
  }
  else if (isIndex(s)) {
    fTall(s, 0);
    fTall(s, 2);
    return;
  }
  else if (isCast(s)) {
    fTall(s, 2);
    return;
  }
  else if (isIcmp(s)) {
    fTall(s, 1);
    fTall(s, 2);
    return;
  }
  assert(0);
}

void callStmt(Sexp* s) {
  if (strcmp(s->list[2]->value, "void") == 0) {
    fCall(s);
    return;
  }

  size_t ignored = _stack_counter++;

  /* create a initializer for the ignored stack variable */
  Sexp* init = makeSexp(makeStr("$%lu", ignored), 0);

  /* create let from call */
  Sexp* let = makeSexp(copyStr("let"), 2);
  let->list[0] = init;
  let->list[1] = s;

  /* replace call with let */
  _block->list[currStmtIndex()] = let;

  /* flatten let */
  Sexp* stmt_cache = _stmt;
  _stmt = let;
  fLet(let);
  _stmt = stmt_cache;
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

    insertStmt(return_sexp, currStmtIndex() + 1);

    /* flatten the call, the return is void */
    fCall(s);
  } else {
    Sexp* return_sexp = makeSexp(copyStr("return"), 2);
    return_sexp->list[0] = s;
    return_sexp->list[1] = sexp(copyStr(return_type->value));

    _block->list[currStmtIndex()] = return_sexp;
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
    callStmt(s);
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
