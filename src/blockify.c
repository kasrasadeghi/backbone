#include "blockify.h"
#include "str.h"
#include <stdio.h>

Sexp* bDo(Sexp *);
Sexp* bIf(Sexp*);
Sexp* bContainer(Sexp*, size_t);
Sexp* bDef(Sexp* s);

void blockify(Sexp* p) {
  for (Sexp** el = p->list; el < p->list + p->length; ++el) {
    if (isDef(*el)) {
      *el = bDef(*el);
    }
  }
}

/**
 * Turns (def FuncN (params ...) Type Stmt* )
 *  =>   (def FuncN (params ...) Type (do ...) )
 */
Sexp* bDef(Sexp* s) {
  return bContainer(s, 3);
}

/**
 * Turns (if Expr Stmt* )
 *  =>   (if Expr (do ...) )
 */
Sexp* bIf(Sexp* s) {
  return bContainer(s, 1);
}

/**
 * Transforms a Container of statements to have one Do-block instead.
 */
Sexp* bContainer(Sexp* s, size_t si) { // si = starting index
  size_t stmt_count = s->length - si;
  Sexp* block = makeSexp(unique("do"), stmt_count);
  for (size_t i = 0; i < stmt_count; ++i) {
    block->list[i] = s->list[si + i];
    s->list[si + i] = NULL;
  }

  block = bDo(block);

  s->length = si + 1;
  s->list[si] = block;
  return s;
}

Sexp* bDo(Sexp *b) {
  for (Sexp** el = b->list; el < b->list + b->length; ++el) {
    if (isIf(*el)) {
      *el = bIf(*el);
    } else if (isDo(*el)) {
      *el = bDo(*el);
    }
  }

  return b;
}





