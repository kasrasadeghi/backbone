
#ifndef __FLATTEN_H__
#define __FLATTEN_H__

#include "parser.h"

void flatten(Sexp* p);

int isCall(Sexp* s);

int isAdd(Sexp* s);

int isIcmp(Sexp* s);

#endif
