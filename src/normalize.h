
#ifndef __FLATTEN_H__
#define __FLATTEN_H__

/**
 * If expressions are the tree's nodes, normalize is flatmap that flattens all the way.
 *
 * The 'normalize' pass takes nested expressions and flattens them
 * to one expression evaluation per statement. Inner expressions are
 * extracted to 'let' statements. Expressions that are expected to be
 * values (literals or registers) are also extracted.
 *
 * This header file also provides an API for common string comparison
 * operations in Sexp's.
 */

#include "sexp.h"

void normalize(Sexp* p);

#endif
