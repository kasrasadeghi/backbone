### Backbone Grammar

```
Binop[x]  -> (x Type Expr Expr)

Program   -> TopLevel*

TopLevel  -> StrTable | Struct | Def | Decl 
StrTable  -> (str-table (int string)*)
Struct    -> (struct StructN (FieldN Type)* )
Decl      -> (decl FuncN (types Type*) Type)
Def       -> (def FuncN (params (ParamN Type)*) Type Stmt* )

Stmt      -> Let | Return | If | CallVargs | Call | Store | Auto
Let       -> (let ValN Expr)
Return    -> (ret Expr Type)
If        -> (if Expr Stmt*)
CallVargs -> (call-vargs FuncN (types Type*) Type (args Expr*))
Call      -> (call       FuncN (types Type*) Type (args Expr*))
Store     -> (store Expr Type Expr)

Expr      -> Call | CallVargs | MathBinop | IComp | Load | Index | Cast | Value
Load      -> (load Type Expr)
Index     -> (index Expr Type Expr)
Cast      -> (cast Type Type Expr)

Value     -> Literal | VarN | StrGet
StrGet    -> (str-get int)
Literal   -> int 

MathBinop -> Add
Add       -> Binop[+]

Icomp     -> LT | LE | GT | GE | EQ | NE
LT        -> Binop[<]
LE        -> Binop[<=] 
GT        -> Binop[>]
GE        -> Binop[>=]
EQ        -> Binop[=]
NE        -> Binop[!=]
```

### Flatten(Backbone) Grammar

//TODO

### Grammar Notes

This is the grammar for a correct structure of S-expressions. Since the
S-expressions are already parsed into a tree, we need some way of 
defining the real structure of our tree and how we expect it to be.

There are several constructs for our grammar. Below they are explained:

 - words that begin with capital letters are NonTerminals
 
 - a Pattern must match some intuitive regex or some defined regex for a string
 
 - a word that begins with a lower case letter is a Word literal. 
   - It is the literal part of the tree that is represented by a Word, 
     typically by a Word found at the beginning of a list.

 - anything that is capitalized and ends in a `N` is a name of something.
 
 - `Binop[x]` is the syntax for a function that takes in a symbol and creates a production