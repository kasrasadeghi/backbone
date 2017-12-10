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

int isCall(Sexp* s) {
  return strcmp(s->value, "call") == 0;
}

int isCallVargs(Sexp* s) {
  return strcmp(s->value, "call-vargs") == 0;
}

int isAdd(Sexp* s) {
  return strcmp(s->value, "+") == 0;
}

int isIcmp(Sexp* s) {
  return strcmp(s->value, "<") == 0
         || strcmp(s->value, "<=") == 0
         || strcmp(s->value, ">") == 0
         || strcmp(s->value, ">=") == 0
         || strcmp(s->value, "=") == 0
         || strcmp(s->value, "!=") == 0;
}

int isDef(Sexp* s) {
  return strcmp(s->value, "def") == 0;
}

int isDecl(Sexp* s) {
  return strcmp(s->value, "decl") == 0;
}

int isLoad(Sexp* s) {
  return strcmp(s->value, "load") == 0;
}

int isIndex(Sexp* s) {
  return strcmp(s->value, "index") == 0;
}