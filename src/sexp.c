#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sexp.h"
#include "str.h"

Sexp* sexp(char* value) {
  Sexp* result = calloc(1, sizeof(Sexp));
  result->value = value;
  result->list = calloc(2, sizeof(Sexp*));
  result->length = 0;
  result->cap = 2;
  return result;
};

Sexp* makeSexp(char* value, size_t length) {
  Sexp* r = malloc(sizeof(Sexp));
  r->value = value;
  r->list = length == 0 ? NULL : calloc(length, sizeof(Sexp*));
  r->length = length;
  r->cap = length;
  return r;
}

Sexp* copySexp(Sexp* s) {
  Sexp* result = makeSexp(copyStr(s->value), s->length);
  for (size_t i = 0; i < s->length; ++i) {
    result->list[i] = copySexp(s->list[i]);
  }
  return result;
}

void incrementLength(Sexp* const s) {
  Sexp* decoy = makeSexp(copyStr("decoy"), 0);
  pushSexp(s, decoy);
  s->list[s->length - 1] = NULL;
  destroySexp(decoy);
}

void _printSexp(Sexp* s, size_t l) {
  for (int i = 0; i < l; ++i) {
    printf("  ");
  }
//  printf("%s %lu/%lu\n", s->value, s->length, s->cap);
  printf("%s\n", s->value);
  for (int i = 0; i < s->length; ++i) {
    _printSexp(s->list[i], l + 1);
  }
}

void printSexp(Sexp* s) {
  _printSexp(s, 0);
}

void pushSexp(Sexp* s, Sexp* child) {
  s->list[s->length] = child;
  ++s->length;
  if (s->length == s->cap) {
    s->cap *= 2;
    s->list = realloc(s->list, s->cap * sizeof(Sexp*));
  }
}

void replaceValue(Sexp* s, char* newValue) {
  free(s->value);
  s->value = newValue;
}

void destroySexp(Sexp* s) {
//  printf("free(%lu): %s\n", (size_t)s->value, s->value); // useful for debugging

  free(s->value);
  for (size_t i = 0; i < s->length; ++i) {
    destroySexp(s->list[i]);
  }
  free(s->list);
  free(s);
}

//region grammar implementation

int isTopLevel(Sexp* s) {
  return isStrTable(s) || isStruct(s) || isDecl(s) || isDef(s);
}
int isStrTable(Sexp* s)  { return strcmp(s->value, "str-table") == 0; }
int isStruct(Sexp* s)    { return strcmp(s->value, "struct") == 0; }
int isDecl(Sexp* s)      { return strcmp(s->value, "decl") == 0; }
int isDef(Sexp* s)       { return strcmp(s->value, "def") == 0; }

int isStmt(Sexp* s) {
  return isLet(s)
         || isReturn(s)
         || isIf(s)
         || isCallVargs(s)
         || isCall(s)
         || isStore(s)
         || isAuto(s)
         || isBecome(s)
      ;
}
int isLet(Sexp* s)       { return strcmp(s->value, "let") == 0; }
int isReturn(Sexp* s)    { return strcmp(s->value, "return") == 0; }
int isIf(Sexp* s)        { return strcmp(s->value, "if") == 0; }
int isCall(Sexp* s)      { return strcmp(s->value, "call") == 0; }
int isCallTail(Sexp* s)  { return strcmp(s->value, "call-tail") == 0; }
int isCallVargs(Sexp* s) { return strcmp(s->value, "call-vargs") == 0; }
int isStore(Sexp* s)     { return strcmp(s->value, "store") == 0; }
int isAuto(Sexp* s)      { return strcmp(s->value, "auto") == 0; }
int isBecome(Sexp* s)    { return strcmp(s->value, "become") == 0; }

int isExpr(Sexp* s) {
  return isCall(s)
         || isCallVargs(s)
         || isMathBinop(s)
         || isIcmp(s)
         || isLoad(s)
         || isIndex(s)
         || isCast(s)
//         || isValue(s) // TODO fix
          ;
}
int isLoad(Sexp* s)      { return strcmp(s->value, "load") == 0; }
int isIndex(Sexp* s)     { return strcmp(s->value, "index") == 0; }
int isCast(Sexp* s)      { return strcmp(s->value, "cast") == 0; }

int isMathBinop(Sexp* s) {
  return isAdd(s);
}
int isAdd(Sexp* s)       { return strcmp(s->value, "+") == 0; }

int isIcmp(Sexp* s) {
  return strcmp(s->value, "<") == 0
         || strcmp(s->value, "<=") == 0
         || strcmp(s->value, ">") == 0
         || strcmp(s->value, ">=") == 0
         || strcmp(s->value, "=") == 0
         || strcmp(s->value, "!=") == 0;
}

//endregion