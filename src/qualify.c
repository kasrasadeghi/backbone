#include <stdio.h>
#include <stdlib.h>
#include "qualify.h"

void qStruct(Sexp* s) {
  for (size_t i = 1; i < s->length; ++i) {
    printSexp(s->list[i], 0);
  }
}

void qDecl(Sexp* s) {
  printf("decl:\n");
  printSexp(s, 0);
}

void qDef(Sexp* s) {
  printf("def:\n");
  printSexp(s, 0);
}

void qTopLevel(Sexp* s) {
  if (isStrTable(s)) {
    // string table does not need qualification
  }
  else if (isStruct(s)) {
    qStruct(s);
  }
  else if (isDecl(s)) {
    qDecl(s);
  }
  else if (isDef(s)) {
    qDef(s);
  }
}

void qualify(Sexp* s) {
  for (size_t i = 0; i < s->length; ++i) {
    qTopLevel(s->list[i]);
  }
}