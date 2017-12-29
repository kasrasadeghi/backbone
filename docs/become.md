# Primitives for Repetition

Repetition is necessary in many parts of this language, and are 
necessary for all languages to be Turing Complete. The difficulty in
repetition is choosing the correct primitive, that allows the
language to be both flexible and efficient.

### The Issue with `for` and `while`

`goto` statements and jumps have been known to make code
harder to read and more difficult to reason about. `break` and
`continue` statements have a similar issue in relation with `while`,
`do...while`, and `for` loops.

Choosing between while and for loops, you have to pick for if you
want one to be written in terms of the other because of how 
`continue` works.

A simple example:

```c
// this loop counts up i
int i = 0;
for (; i < 10; ++i) {
  continue;
}
```

```c
// this loop stalls infinitely
int i = 0;
while (i < 10) {
  continue;
  ++i;
}
```

The `for` loop has an extra statement that runs in between the 
beginning of the loop's body and the continue.

`continue` and `break` statements have strange semantics that
do not lead cleanly to nesting, and while some would argue that
nesting too many loops is bad style anyways (and I would agree),
disambiguating the statements with labels is certainly a relic
from times long past. These strange semantics and inability to
nest comes from their similarity to `goto`s. 
 
There should be a better way.

### Tail Recursion as a Primitive

Tail recursion seems like a better primitive for repetition.
While some may argue that using recursion everywhere you want
to repeat is confusing and that recursion is hard, I disagree.
I think that recursive solutions are often more elegant and that
because you can represent higher order primitives like map, filter,
and reduce using recursion, it is an easier primitive to use.

It's also easier to type-check, which will probably be useful in
the future. 

A lot of languages support tail recursion as an optimization, 
but I would prefer to have it a distinct language feature. If 
you write a function that you expect to undergo Tail Call 
Elimination, you should be able to say that and it should warn 
you when that does not happen. Because you are always
returning the result of the call in Tail Recursion, they should be
combined in one statement.

```
(ret (tailcall funcName (types ...) retType (args ...)) retType)
```

vs

```
(become funcName (types ...) retType (args ...))
```