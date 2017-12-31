
#ifndef __NORMALIZE_H__
#define __NORMALIZE_H__

/**
 * The 'normalize' pass takes nested expressions and flattens them
 * to one expression evaluation per statement. Inner expressions are
 * extracted to 'let' statements. Expressions that are expected to be
 * values (literals or registers) are also extracted.
 */

#include "sexp.h"

void normalize(Sexp* p);

#endif
