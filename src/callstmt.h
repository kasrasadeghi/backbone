
#ifndef __CALL_STMT_H__
#define __CALL_STMT_H__

#include "sexp.h"

/**
 * Transforms expr-calls (calls that do not return void) to be statements.
 * Is a no-op on calls that return void.
 *
 * ('CallLike 'name 'types 'return-type 'args)
 *
 * if 'return-type == void
 * => ('CallLike 'name 'types 'return-type 'args)
 *
 * else
 * => (let 'new-local ('CallLike 'name 'types 'return-type 'args)
 *
 * @param p - a program with calls in it.
 */
void callStmt(Sexp* p);

#endif