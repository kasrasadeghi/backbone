#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sexp.h"
#include "str.h"

Sexp* sexp(char* value) {
  Sexp* result = calloc(1, sizeof(Sexp));
  result->value = value;
  result->list = calloc(2, sizeof(Sexp*));
  result->len = 0;
  result->cap = 2;
  return result;
};

Sexp* makeSexp(char* value, size_t length) {
  Sexp* r = malloc(sizeof(Sexp));
  r->value = value;
  r->list = length == 0 ? NULL : calloc(length, sizeof(Sexp*));
  r->len = length;
  r->cap = length;
  return r;
}

Sexp* copySexp(Sexp* s) {
  Sexp* result = makeSexp(copyStr(s->value), s->len);
  for (size_t i = 0; i < s->len; ++i) {
    result->list[i] = copySexp(s->list[i]);
  }
  return result;
}

size_t indexOfSexp(Sexp* parent, Sexp* child) {
  size_t csi = 0;
  for (; csi < parent->len; ++csi) { // curr statement index = csi
    if (parent->list[csi] == child) break;
  }
  if (csi == parent->len) {
    fprintf(stderr, "backbone: indexing error: child was not found in parent's list");
    exit(1);
  }

  return csi;
}

void insertSexp(Sexp* parent, Sexp* stmt, size_t csi) {
  incrementLength(parent);

  /* move everything from [csi, length) over, starting from the end */
  for (size_t si = parent->len - 1; si >= csi; --si) {
    parent->list[si] = parent->list[si - 1];
    if (si == 0) break; // prevent size_t rollover from 0 to extremely positive value
  }

  /* place statement in parent */
  parent->list[csi] = stmt;
}

void incrementLength(Sexp* const s) {
  pushSexp(s, NULL);
}

void _printSexp(Sexp* s, size_t l) {
  for (int i = 0; i < l; ++i) {
    printf("  ");
  }
  if (s == NULL) {
    printf("NULL\n");
    return;
  }
  if (s->value == NULL) {
    printf("NULL VALUE\n");
  } else {
    printf("%s\n", s->value);
//    printf("%s %lu/%lu\n", s->value, s->len, s->cap);
  }
  for (int i = 0; i < s->len; ++i) {
    _printSexp(s->list[i], l + 1);
  }
}

void printSexp(Sexp* s) {
  _printSexp(s, 0);
}

void pushSexp(Sexp* const s, Sexp* child) {
  if (s->len == s->cap) {
    s->cap *= 2;
    s->list = realloc(s->list, s->cap * sizeof(Sexp*));
  }

  s->list[s->len] = child;
  ++s->len;
}

void replaceValue(Sexp* s, char* newValue) {
  free(s->value);
  s->value = newValue;
}

void destroySexp(Sexp* s) {
//  printf("free(%lu): %s\n", (size_t)s->value, s->value); // useful for debugging

  free(s->value);
  for (size_t i = 0; i < s->len; ++i) {
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

int isCallLike(Sexp* s)  {
  return isCall(s) || isCallTail(s) || isCallVargs(s) || isBecome(s);
}
int isCall(Sexp* s)      { return strcmp(s->value, "call") == 0; }
int isCallTail(Sexp* s)  { return strcmp(s->value, "call-tail") == 0; }
int isCallVargs(Sexp* s) { return strcmp(s->value, "call-vargs") == 0; }
int isBecome(Sexp* s)    { return strcmp(s->value, "become") == 0; }

int isStmt(Sexp* s) {
  return isLet(s)
         || isReturn(s)
         || isIf(s)
         || isStore(s)
         || isAuto(s)
         || isCallLike(s);
      ;
}
int isLet(Sexp* s)       { return strcmp(s->value, "let") == 0; }
int isReturn(Sexp* s)    { return strcmp(s->value, "return") == 0; }
int isIf(Sexp* s)        { return strcmp(s->value, "if") == 0; }
int isStore(Sexp* s)     { return strcmp(s->value, "store") == 0; }
int isAuto(Sexp* s)      { return strcmp(s->value, "auto") == 0; }
int isDo(Sexp* s)        { return strcmp(s->value, "do") == 0; }

int isExpr(Sexp* s) {
  return isTall(s)
//         || isValue(s) // TODO fix
          ;
}
int isTall(Sexp* s) {
  return isCallLike(s)
         || isMathBinop(s)
         || isIcmp(s)
         || isLoad(s)
         || isIndex(s)
         || isCast(s)
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

int isValue(Sexp* s) {
  return isLiteral(s) || isStrGet(s); // TODO: OR is not a keyword? what's the distinguishing factor for register lookup?
}
int isLiteral(Sexp* s)   { return isBool(s) || isdigit(s->value[0]); }
int isBool(Sexp* s)      { return strcmp(s->value, "true") == 0
                                  || strcmp(s->value, "false") == 0; }
int isStrGet(Sexp* s)    { return strcmp(s->value, "str-get") == 0; }

int isVoid(Sexp* s)      { return strcmp(s->value, "void") == 0; }

//endregion