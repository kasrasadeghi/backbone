#include <stdio.h>
#include "normalize.h"
#include "str.h"

Sexp* nDef(Sexp*);
Sexp* nDo(Sexp*, Sexp*);
Sexp* nIf(Sexp* block, size_t csi);

/**
 * Expressions that are expected to be values are normalized,
 * so we need to dig through and figure out which
 * parts of the tree either are expressions or contain expressions.
 */
void normalize(Sexp* s) {
  for (Sexp** el = s->list; el < s->list + s->length; ++el) {
    if (isDef(*el)) {
      *el = nDef(*el);
    }
  }
}

static size_t _local_count = 0;

Sexp* newLocal() {
  size_t local = _local_count++;
  char cache[20];
  snprintf(cache, 20, "$%lu", local);
  char* value = str_copy(cache);
  Sexp* result = makeSexp(value, 0);
  return result;
};

Sexp* nDef(Sexp* def) {
  _local_count = 0;
  def->list[3] = nDo(def, def->list[3]);
  return def;
}

Sexp* nDo(Sexp* def, Sexp* block) {
  for (size_t i = 0; i < block->length; ++i) {
    if (isIf(block->list[i])) {
      block = nIf(block, i);
    }
  }
  return def;
}

Sexp* nIf(Sexp* block, size_t csi) { // csi = current statement index
  Sexp* s = block->list[csi];
  Sexp* cond = s->list[0];
  if (isTall(cond)) {
    /* we need to extract the condition to a new let-stmt */
    Sexp* let = NULL;
    {
      Sexp* name = newLocal();

      /* create a new let-stmt */
      let = makeSexp(str_copy("let"), 2);
      let->list[0] = name;
      let->list[1] = cond;

      /* replace the condition with a copy of the name */
      s->list[0] = copySexp(name);
    }

    /* then insert the let-stmt into the block before the current statement */
    {
      /* increase the size of the list */
      incrementLength(block);

      /* move [csi, length) to the right by one */
      for (size_t si = block->length - 1; si != -1; --si) {
        printf("%lu %lu\n", si, csi);
        block->list[si] = block->list[si - 1];
      }

      /* insert the statement into block[csi] */
      block->list[csi] = let;
    }
  }
  return block;
}