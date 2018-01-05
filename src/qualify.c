#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "qualify.h"
#include "str.h"

void qBlock(Sexp* s, size_t startIndex);
void qCall(Sexp*);

/**
 * Takes a type Sexp and qualifies non-primitive types.
 */
void qType(Sexp* s) {
  char* type = s->value;

  if (strcmp(type, "void") == 0 || strcmp(type, "...") == 0) {
    return;
  }

  char *primitive_types[] = {"i1", "i8", "i32", "i64", "u1", "u8", "u32", "u64"};

  for (int i = 0; i < 8; i++) {
    char *prim = primitive_types[i];
    size_t prim_len = strlen(prim);
    if (strncmp(type, prim, prim_len) == 0) {
      if (*(type + prim_len) == '\0'      // if it is exactly equal
          || *(type + prim_len) == '*') { // or if it is a pointer  //TODO handle i8** and arbitrary pointer redirection
        // primitive type
        s->value[0] = 'i';
        return;
      }
    }
  }

  s->value = makeStr("%%struct.%s", type);
  free(type);
}

void qTypes(Sexp* s) {
  for (size_t i = 0; i < s->length; ++i) {
    qType(s->list[i]);
  }
}

void qExpr(Sexp* s) {
  if (isCall(s) || isCallVargs(s) || isCallTail(s) || isBecome(s)) {
    qCall(s);
  } else if (isMathBinop(s) || isIcmp(s)) {
    qType(s->list[0]);
    qExpr(s->list[1]);
    qExpr(s->list[2]);
  } else if (isLoad(s)) {
    qType(s->list[0]);
    qExpr(s->list[1]);
  } else if (isIndex(s)) {
    qExpr(s->list[0]);
    qType(s->list[1]);
    qExpr(s->list[2]);
  } else if (isCast(s)) {
    qType(s->list[0]);
    qType(s->list[1]);
    qExpr(s->list[2]);
  }
}

void qCall(Sexp* s) {
  qTypes(s->list[1]);
  qType(s->list[2]);
  Sexp* args = s->list[3];
  for (size_t i = 0; i < args->length; ++i) {
    qExpr(args->list[i]);
  }
}

void qStmt(Sexp* s) {
  if (isLet(s)) {
    qExpr(s->list[1]);
  } else if (isReturn(s) && strcmp(s->list[0]->value, "void") != 0) {
    qExpr(s->list[0]);
    qType(s->list[1]);
  } else if (isIf(s)) {
    qExpr(s->list[0]);
    qBlock(s, 1);
  } else if (isStore(s)) {
    qExpr(s->list[0]);
    qType(s->list[1]);
    qExpr(s->list[2]);
  } else if (isAuto(s)) {
    qType(s->list[1]);
  } else if (isCallLike(s)) {
    qCall(s);
  }
}

void qBlock(Sexp* s, size_t startIndex) {
  for (size_t i = startIndex; i < s->length; ++i) {
    qStmt(s->list[i]);
  }
}

void qStruct(Sexp* s) {
  for (size_t i = 1; i < s->length; ++i) {
    qType(s->list[i]->list[0]);
  }
}

void qDecl(Sexp* s) {
  qTypes(s->list[1]);
  qType(s->list[2]);
}

void qParams(Sexp* s) {
  for (size_t i = 0; i < s->length; ++i) {
    qType(s->list[i]->list[0]);
  }
}

void qDef(Sexp* s) {
  qParams(s->list[1]);
  qType(s->list[2]);
  qBlock(s, 3);
}

void qTopLevel(Sexp* s) {
  if (isStruct(s)) {
    qStruct(s);
  }
  else if (isDecl(s)) {
    qDecl(s);
  }
  else if (isDef(s)) {
    qDef(s);
  } else {
    assert(isStrTable(s));
  }
}

void qualify(Sexp* s) {
  for (size_t i = 0; i < s->length; ++i) {
    qTopLevel(s->list[i]);
  }
}