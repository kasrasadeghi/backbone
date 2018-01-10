#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "locals.h"

static size_t _stack_counter;
static int _found_stack = 0;

void countDo(Sexp* s);

void countLet(Sexp* let) {
  char* name = let->list[0]->value;
  if (strlen(name) == 0) {
    printSexp(let);
    fprintf(stderr, "backbone: ERROR: name for let is empty\n");
    exit(1);
  }
  if (name[0] != '$') return; // not a generated local

  /* get size_t size from (char*)(name + 1) */
  size_t size = 0;
  char* curr = name + 1;
  size_t base = 1;
  while (*curr) {
    assert(isdigit(*curr));
    size += (*curr - '0') * base;

    base *= 10;
    curr++;
  }

  if (!_found_stack) {
    _found_stack = 1;
    _stack_counter = 0;
  } else if (size > _stack_counter) {
    _stack_counter = size;  //TODO replace with max
  }
}

void countStmt(Sexp* s) {
  if (isLet(s)) {
    countLet(s);
  }
  else if (isDo(s)) {
    countDo(s);
  }
  else if (isIf(s)) {
    countDo(s->list[1]);
  }
  else {
    /* statements without possibly tall expressions in them */
    int matchAllStmt = isAuto(s) || isCallLike(s) || isStore(s) || isReturn(s);
    if (!matchAllStmt) {
      printSexp(s);
    }
    assert(matchAllStmt);
  }
}

void countDo(Sexp* s) {
  for (int i = 0; i < s->length; ++i) {
    countStmt(s->list[i]);
  }
}

void initLocals(Sexp* def) {
  _found_stack = 0;
  countDo(def->list[3]);

  if (!_found_stack) {
    _stack_counter = 0;
  } else {
    _stack_counter++;
  }

}

size_t newLocal() {
  return _stack_counter++;
}