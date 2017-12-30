#include "flatten.h"

Sexp* fDef(Sexp* p) {

}

void flatten(Sexp* p) {
  for (Sexp** i = p->list; i < p->list + p->length; ++i) {
    if (isDef(*i)) {
      *i = fDef(*i);
    }
  }
}
