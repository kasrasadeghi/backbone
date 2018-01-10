### Variable names come before types
Variable names should be the center of attention, so they should come before their 
corresponding types.


### Expressions Vs Values
All values are expressions, but not all expressions are values.
Values are literals and registers.

### Adding Expressions
When adding an expression that is not a value, make sure to 
 - add it to isTall() in normalize.c
 - add it to sexp.c and sexp.h as an isWhateverName(Sexp* s)
 - add it to gExpr in gen_llvm.c
 - add it to nLet() if it is Tall. see [Names are sometimes Values](#namevalue).

### Adding Statements
When adding a statement:
 - if it has an expression that can be tall, add it to nBlock
 - if not, add it to the assert at the end of nBlock
 - add it to nStmt

### Place an assert every time you hunt down a bug.

### Look for the most powerful assert you can place.

### Prefer early returns to nested if statements.

### <a name="namevalue">Names are sometimes Values.</a>
When thinking about adding an expression, if you refer to some child of the expression
as a "Name" of a value, then expand it to allow for all expressions by adding it 
to nLet as a Tall Expression.