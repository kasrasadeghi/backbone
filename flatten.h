
#ifndef __FLATTEN_H__
#define __FLATTEN_H__

/**
 * The 'flatten' pass takes nested expressions and flattens them
 * to one expression evaluation per statement. Inner expressions are
 * extracted to 'let' statements. Expressions that are expected to be
 * values (literals or registers) are also extracted.
 *
 * This header file also provides an API for common string comparison
 * operations in Sexp's.
 */

#include "parser.h"
#include "sexp.h"

void flatten(Sexp* p);

#endif
