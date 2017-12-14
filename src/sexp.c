#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sexp.h"

Sexp* sexp(char* value) {
  Sexp* result = calloc(1, sizeof(Sexp));
  result->value = value;
  result->list = calloc(2, sizeof(Sexp*));
  result->length = 0;
  result->cap = 2;
  return result;
};

void printSexp(Sexp* s, int l) {
  for (int i = 0; i < l; ++i) {
    printf("  ");
  }
  printf("%s\n", s->value);
  for (int i = 0; i < s->length; ++i) {
    printSexp(s->list[i], l + 1);
  }
}

void pushSexp(Sexp* s, Sexp* child) {
  s->list[s->length] = child;
  ++s->length;
  if (s->length == s->cap) {
    s->cap *= 2;
    s->list = realloc(s->list, s->cap * sizeof(Sexp*));
  }
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
         || isAuto(s);
}
int isLet(Sexp* s)       { return strcmp(s->value, "let") == 0; }
int isReturn(Sexp* s)    { return strcmp(s->value, "return") == 0; }
int isIf(Sexp* s)        { return strcmp(s->value, "if") == 0; }
int isCall(Sexp* s)      { return strcmp(s->value, "call") == 0; }
int isCallVargs(Sexp* s) { return strcmp(s->value, "call-vargs") == 0; }
int isStore(Sexp* s)     { return strcmp(s->value, "store") == 0; }
int isAuto(Sexp* s)      { return strcmp(s->value, "auto") == 0; }

int isExpr(Sexp* s) {
  return isCall(s)
         || isCallVargs(s)
         || isMathBinop(s)
         || isIcmp(s)
         || isLoad(s)
         || isIndex(s)
         || isCast(s)
         || isValue(s);
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