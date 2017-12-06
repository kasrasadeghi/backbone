#include <string.h>
#include "flatten.h"

void fDef(Sexp* d) {
  for (int i = 3; i < d->length; ++i) {
    Sexp* statement = d->list[i];
    if (strcmp(statement->value, "let") == 0) {

    } else if (strcmp(statement->value, "return") == 0) {

    }
  }
}

void flatten(Sexp* p) {
  for (int i = 0; i < p->length; ++i) {
    Sexp* child = p->list[i];
    if (strcmp(child->value, "def") == 0) {
      fDef(child);
    }
  }
}