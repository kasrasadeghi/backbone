
#ifndef __LOCALS_H__
#define __LOCALS_H__

#include <stdlib.h>
#include "sexp.h"

/**
 * Requires blockify
 */

void initLocals(Sexp* def);
size_t newLocal();

#endif