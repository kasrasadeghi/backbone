#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "normalize.h"
#include "str.h"
#include "locals.h"

//region Forward Declarations

void nBlock(Sexp* block);
void nLet(Sexp* block, Sexp* let);

//endregion

/**
 * Extracts an expression from a Sexp s at index index.
 *
 *  - Replaces the expression with a reference to a new local.
 *  - Creates a let expression that initializes the local, setting it to the extracted sexp.
 *
 *
 * @param s
 * @param index
 * @return the let expression to be inserted into the current definition, before the current stmt.
 */
Sexp* extractLet(Sexp* const s, const int index) {
  Sexp* expr = s->list[index];
  size_t local = newLocal();

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
void nTall(Sexp* block, Sexp* stmt, Sexp* parent, int i) {
  if (isTall(parent->list[i])) {
    Sexp* let = extractLet(parent, i);
    size_t csi = indexOfSexp(block, stmt);
    insertSexp(block, let, csi);

    /* recurse on the let we just inserted */
    nLet(block, let);
  }
}

/**
 * Takes a block, a statement, and a call. Goes through the arguments of the call and flattens them.
 *
 * The arguments that actually needed flattening are placed in the block before stmt.
 */
void nCall(Sexp* block, Sexp* stmt, Sexp* call) {
  Sexp* args = call->list[3];
  for (int ai = 0; ai < args->length; ++ai) { // argument index = ai
    nTall(block, stmt, args, ai);
  }
}

/**
 * tall expression = expression that contains another expression
 * this should flatten every tall expression
 */
void nLet(Sexp* block, Sexp* let) {
  Sexp* expr = let->list[1];
  if (isCall(expr) || isCallVargs(expr) || isCallTail(expr)) { //TODO use isCallLike && !isBecome
    nCall(block, let, expr);
    return;
  }
  else if (isAdd(expr)) {
    nTall(block, let, expr, 1);
    nTall(block, let, expr, 2);
    return;
  }
  else if (isLoad(expr)) {
    nTall(block, let, expr, 1);
    return;
  }
  else if (isIndex(expr)) {
    nTall(block, let, expr, 0);
    nTall(block, let, expr, 2);
    return;
  }
  else if (isCast(expr)) {
    nTall(block, let, expr, 2);
    return;
  }
  else if (isIcmp(expr)) {
    nTall(block, let, expr, 0);
    nTall(block, let, expr, 1);
    return;
  }
  assert(0);
}

void nStmt(Sexp* block, Sexp* s) {
  if (isLet(s)) {
    nLet(block, s);
  }
  else if (isDo(s)) {
    nBlock(s);
  }
  else if (isReturn(s)) {
    if (s->length == 2) {
      nTall(block, s, s, 0);
    }
    // else { for void returns, do nothing. }
  }
  else if (isIf(s)) {
    nTall(block, s, s, 0);
    nBlock(s->list[1]);
  }
  else if (isCallLike(s)) {
    assert(strcmp(s->list[2]->value, "void") == 0);
    assert(!isBecome(s)); // become should no longer exist because become.c comes before
    nCall(block, s, s);
  }
  else if (isStore(s)) {
    /* (store Value Type Ptr) */
    nTall(block, s, s, 0);
    nTall(block, s, s, 2);
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

void nBlock(Sexp* block) {
  for (int i = 0; i < block->length; ++i) {
    nStmt(block, block->list[i]);
  }
}

void fDef(Sexp* s) {
  initLocals(s);
  nBlock(s->list[3]);
}

void normalize(Sexp* p) {
  for (int i = 0; i < p->length; ++i) {
    Sexp* child = p->list[i];
    if (isDef(child)) {
      fDef(child);
    }
  }
}
