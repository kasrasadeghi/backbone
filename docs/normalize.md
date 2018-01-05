### Normalization

#### Requirements

Normalization requires `blockify`, `callStmt`, and `become`.

#### Placement

All expressions that are expected to be values are normalized.

Which expressions are expected to be values?

Exprs:
 - Binop    at `[1]` and `[2]` to be values.
 - CallLike at children of `args` (where `args` = `[3]`) to contains values for all `[0..]`.
 - Index    at `[0]` and `[2]`
 - Load     at `[1]`
 
Stmts:
 - Store    at `[0]` and `[2]`
 - Return   at `[0]` when `[0]` != "void"
 - If       at `[0]`

Let does does expect `[1]` to be a value, and that is the only place where Expr is not flattened. 

#### Procedure: How do we normalize?


 
#### Pre-Grammar

```
Program   -> TopLevel*

TopLevel  -> StrTable | Struct | Def | Decl 
StrTable  -> (str-table (int string)*)
Struct    -> ...
Decl      -> ...
Def       -> (def FuncN (params (ParamN Type)*) Type Do )

Stmt      -> Let | Return | If | Store | Auto | Do
Do        -> (do Stmt*)
Let       -> (let ValN Expr)
If        -> (if Expr Do)
Return    -> (return Expr Type) | (return void)
Store     -> (store Expr Type Expr)
Auto      -> (auto VarN Type)

Expr      -> CallLike | Binop | Load | Index | Cast | Value
Load      -> (load Type Expr)
Index     -> (index Expr Type Expr)
Cast      -> (cast Type Type Expr)

CallLike  -> CallF[x] for x in { call, call-vargs, call-tail } 
CallF[x]  -> (x FuncN (types Type*) Type (args Expr*))

Binop     -> BinopF[x] for x in { +, <, <=, >, >=, =, != } 
BinopF[x] -> (x Type Expr Expr)
```