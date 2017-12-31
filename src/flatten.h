
#ifndef __FLATTEN_H__
#define __FLATTEN_H__

/**
 * The 'flatten' pass takes nested blocks and expands them to fall under
 * block containers (at the time of writing those are definitions and if-statements).
 */

#include "sexp.h"

void flatten(Sexp* p);

#endif
