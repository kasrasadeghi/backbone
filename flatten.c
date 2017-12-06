#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "flatten.h"

int isCall(Sexp* s) {
  return strcmp(s->value, "call") == 0;
}

int isAdd(Sexp* s) {
  return strcmp(s->value, "+") == 0;
}

/**
 * returns true for expressions that are not values.
 */
int unflat(Sexp* s) {
  return isCall(s) || isAdd(s);
}

static Sexp* _p = NULL;
static int   _defi = 0;
static Sexp* _def = NULL;
static Sexp* _stmt = NULL;
static size_t _stack_counter = 0;

void fLet(Sexp* s);

void fCall(Sexp* s) {
  Sexp* args = s->list[3];
  for (int ai = 0; ai < args->length; ++ai) { // argument index = ai
    if (unflat(args->list[ai])) {
      Sexp* arg = args->list[ai];
      size_t local = _stack_counter++;

      /* replace with reference to local */
      char* string = calloc(12, 1);
      snprintf(string, 12, "$%lu", local);
      Sexp* ref = calloc(1, sizeof(Sexp));
      ref->value = string;
      args->list[ai] = ref;

      /* make an initializer of the local */
      char* string_copy = malloc(strlen(string) + 1);
      strcpy(string_copy, string);
      Sexp* init = calloc(1, sizeof(Sexp));
      init->value = string_copy;

      Sexp* let = calloc(1, sizeof(Sexp));
      let->value = "let";
      let->list = calloc(2, sizeof(Sexp*));
      let->length = 2;
      let->cap = 2;
      let->list[0] = init;
      let->list[1] = arg;

      /* now we have to insert the let statement before the current statement */
      int csi = 0;
      for (; csi < _def->length; ++csi) { // curr statement index = csi
        if (_def->list[csi] == _stmt) break;
      }
      if (csi == _def->length) {
        fprintf(stderr, "backbone: could not find current statement in definition");
        exit(1);
      }
      /* make room: should increase length by one */
      if (_def->length == _def->cap) {
        _def = realloc(_def, (_def->cap + 1) * sizeof(Sexp*));
        _p->list[_defi] = _def;
        _def->cap = _def->cap + 1;
        _def->length = _def->cap;
      } else {
        assert(_def->length < _def->cap);
        _def->length = _def->length + 1;
      }

      /* move everything from [csi, length) over, starting from the end */
      for (size_t si = _def->length - 1; si >= csi; --si) {
        _def->list[si] = _def->list[si - 1];
      }
      _def->list[csi] = let;

      /* recurse on the let we just inserted */
      Sexp* stmt_cache = _stmt;
      _stmt = let;
      fLet(let);
      _stmt = stmt_cache;
    }
  }
}

void fAdd(Sexp* s) {

}

void fLet(Sexp* s) {
  if (isCall(s->list[1])) {
    fCall(s->list[1]);
  } else if (isAdd(s->list[1])) {
    fAdd(s->list[1]);
  }
}

void fReturn(Sexp* s) {
  if (unflat(s->list[0])) {

  }
}

void fDef(Sexp* s) {
  _stack_counter = 0;
  _def = s;

  for (int i = 3; i < s->length; ++i) {
    Sexp* statement = s->list[i];
    _stmt = statement;

    //TODO call-as-a-statement
    if (strcmp(statement->value, "let") == 0) {
      fLet(statement);
    } else if (strcmp(statement->value, "return") == 0) {
      fReturn(statement);
    }
  }
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
