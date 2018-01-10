
### Overview

Flatten is unique in that it is the only Sexp* to Sexp* transformation. It is unlike the 
other phases, Parser and Gen, in that it is an 
[operator](https://en.wikipedia.org/wiki/Operator_(mathematics)). This makes it a pure 
transformation of trees, and it is how we implement compiler passes.

The structure of our language, compared to the LLVM Intermediate Representation language 
(LLVM IR),gives more flexibility to the expression-value relationship. In LLVM IR, 
there is one expression evaluation per instruction, as instruction evaluation is 
thought of as an opcode. In our language, we generalize the strictness of the LLVM IR 
in order to allow for a more natural expression of ideas and a way to elide the needless
use of extra variable names and register management.

In our language, we separate Expressions that are immediate Values or Expressions that
can have Expressions inside of them, which we call Tall Expressions (also Unflat Expressions).

```
Expression = Value | Tall
```

In LLVM IR, there are 
[first-class types](https://llvm.org/docs/LangRef.html#first-class-types).
These correspond to our language's Value Expressions. It often requires that the arguments
to an opcode be first-class types. For example, every argument in the argument list in a
function call instruction must be a first-class type. Our language defines call's arguments
as expressions, and then it flattens each argument [(nTall)]("#ftall") if it is not an 
immediate value.

### normalize: Flattening the Program

Flattening a program basically boils down to flattening each of it's blocks, which
correspond to LLVM control blocks. Each of Backbone's blocks must end in a return, while
in LLVM there are other kinds of terminal instructions.

### nTall: Flattening a Tall Expression

Flattening a Tall Expression extracts the expression and places it in a let. To create a
value for the let, we have a global counter for the whole normalize call. Local variable
names only have to be unique in each function definition, so we reset the counter each
time we normalize a function definition. Once we extract the let, we insert the let before
the statement we extracted it from with the name received from the local counter (e.g. $5).

To keep invariants consistent, the size of the encompassing block must be increased by one,
and with the introduction of if-statements we have recursive search through blocks, so we
have to keep track of the current block and cache it every time we recurse inwards 
(nBlock).

Once we've inserted the let statement, we also need to recurse into it to make sure that
the expression we just flattened does not need further flattening [(nLet)]("#flet").

### <a name="flet">nLet</a>: Flattening Let Statements

Flattening let statements is where we do the bulk of the initial investigation into flattening
expressions. We check to see if the child of each let is one of the expressions that can have
expression children, i.e. they are Tall. This is also where we do [callStmt]("#callstmt")

### <a name="callstmt">callStmt</a>: Call-as-a-Statement

Flatten also has a Call-as-a-Statement call as it traverses through statements as a 
convenience. Sometimes we call a function and we want to ignore the result. So we just 
place the call into a let, replace it, and use the local counter that normalize has available to
it to ease the work.