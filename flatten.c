#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "flatten.h"

/**
 * returns true for expressions that are not values.
 */
int isTall(Sexp* s) {
  return isCall(s) || isAdd(s) || isIcmp(s) || isLoad(s) || isIndex(s) || isCast(s);
}

static Sexp* _p = NULL;
static int   _defi = 0;
static Sexp* _block = NULL;
static Sexp* _stmt = NULL;
static size_t _stack_counter = 0;

/**
 * Inserts a statement into the current definition, _def.
 *
 *  - Increases the length of _def by one.
 *  - Globals: _defi, _def, _p
 *
 * Note: relies on the lack of nested definitions.
 *
 * @param stmt  - the statement to insert.
 * @param index - the index to insert it at.
 */
static void insertStmt(Sexp* stmt, int index) {
  /* make room for another statement in the current definition */
  /* should increase the length by 1 */
  if (_block->length == _block->cap) {
    _block = realloc(_block, (_block->cap + 1) * sizeof(Sexp*));
    _p->list[_defi] = _block;
    _block->cap = _block->cap + 1;
    _block->length = _block->cap;
  } else {
    assert(_block->length < _block->cap);
    _block->length = _block->length + 1;
  }

  /* move everything from [csi, length) over, starting from the end */
  for (size_t si = _block->length - 1; si >= index; --si) {
    _block->list[si] = _block->list[si - 1];
  }
  _block->list[index] = stmt;
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
  Sexp* arg = s->list[index];
  size_t local = _stack_counter++;

  /* replace with reference to local */
  char* string = calloc(12, 1);
  snprintf(string, 12, "$%lu", local);
  Sexp* ref = calloc(1, sizeof(Sexp));
  ref->value = string;
  s->list[index] = ref;

  /* make an initializer of the local */
  char* string_copy = malloc(strlen(string) + 1);
  strcpy(string_copy, string);
  Sexp* init = calloc(1, sizeof(Sexp));
  init->value = string_copy;

  /* create a let to insert into the definition */
  Sexp* let = calloc(1, sizeof(Sexp));
  let->value = "let";
  let->list = calloc(2, sizeof(Sexp*));
  let->length = 2;
  let->cap = 2;
  let->list[0] = init;
  let->list[1] = arg;

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

void fLet(Sexp* s);

void fIf(Sexp* s);

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
//endregion

void fCall(Sexp* s) {
  Sexp* args = s->list[3];
  for (int ai = 0; ai < args->length; ++ai) { // argument index = ai
    fTall(args, ai);
  }
}

void fAdd(Sexp* s) {
  fTall(s, 1);
  fTall(s, 2);
}

void fLoad(Sexp* s) {
  fTall(s, 1);
}

void fIndex(Sexp* s) {
  fTall(s, 0);
  fTall(s, 2);
}

void fCast(Sexp* s) {
  fTall(s, 2);
}

/* this should flatten every expression that contains another expression */
void fLet(Sexp* s) {
  if (isCall(s->list[1]) || isCallVargs(s->list[1])) {
    fCall(s->list[1]);
    return;
  }
  else if (isAdd(s->list[1])) {
    fAdd(s->list[1]);
    return;
  }
  else if (isLoad(s->list[1])) {
    fLoad(s->list[1]);
    return;
  }
  else if (isIndex(s->list[1])) {
    fIndex(s->list[1]);
    return;
  }
  else if (isCast(s->list[1])) {
    fCast(s->list[1]);
    return;
  }
  assert(0);
}

void fReturn(Sexp* s) {
  fTall(s, 0);
}

/* (store Value Type PtrName) */
void fStore(Sexp* s) {
  fTall(s, 0);
  fTall(s, 2);
}

void callStmt(Sexp* s) {
  if (strcmp(s->list[2]->value, "void") == 0) {
    return;
  }

  size_t ignored = _stack_counter++;

  /* create a initializer for the ignored stack variable */
  char* string = calloc(12, 1);
  snprintf(string, 12, "$%lu", ignored);
  Sexp* init = calloc(1, sizeof(Sexp));
  init->value = string;

  /* create let from call */
  Sexp* let = calloc(1, sizeof(Sexp));
  let->value = "let";
  let->list = calloc(2, sizeof(Sexp*));
  let->length = 2;
  let->cap = 2;
  let->list[0] = init;
  let->list[1] = s;

  /* replace call with let */
  int csi = currStmtIndex();
  _block->list[csi] = let;

  /* flatten let */
  Sexp* stmt_cache = _stmt;
  _stmt = let;
  fLet(let);
  _stmt = stmt_cache;
}

void fBlock(Sexp* s, int startIndex) {
  Sexp* block_cache = _block;
  _block = s;

  for (int i = startIndex; i < s->length; ++i) {
    Sexp* statement = s->list[i];
    _stmt = statement;

    if (strcmp(statement->value, "let") == 0) {
      fLet(statement);
    }
    else if (strcmp(statement->value, "return") == 0) {
      fReturn(statement);
    }
    else if (strcmp(statement->value, "if") == 0) {
      fIf(statement);
    }
    else if (isCall(s) || strcmp(statement->value, "call-vargs") == 0) {
      callStmt(statement);
    }
    else if (strcmp(statement->value, "store") == 0) {
      fStore(statement);
    }
  }
  _block = block_cache;
}

void fIf(Sexp* s) {
  fTall(s, 0);

  fBlock(s, 1);
}

void fDef(Sexp* s) {
  _stack_counter = 0;
  fBlock(s, 3);
}

void flatten(Sexp* p) {
  _p = p;
  for (int i = 0; i < p->length; ++i) {
    Sexp* child = p->list[i];
    if (strcmp(child->value, "def") == 0) {
      _defi = i;
      fDef(child);
    }
  }
}
