
### Parser Overview

The parser of most languages is fairly tedious to write and a great deal of research has
gone into writing efficient parsers. There's a great number of parsing algorithms to choose
from, but most big projects roll their own parser or use a parser-lexer framework like
flex/bison (or lex/yacc). We don't have a back-end for any of the frameworks because our 
language doesn't exist yet, so it's a better idea to write a very simple parser for a language
with very simple structure.

It would be a tremendous time investment to write a parser for our language in C and then write
it again in our language if we were to choose a complex structure for the parser. There are two
language structures that are fairly simple to parse. Concatenative languages like Forth or Factor
have a very simple structure. Each token is separated by a space, and the evaluation of the
language is token by token, in-order (most of the time). The other choice is a LISP-like language.
We chose to use a LISP-like language structure because it is more natural to express ideas in that
are already structured like a tree, and our bootstrapping language is C which has a tree-based
structure. It also doesn't allow for ambiguous parses, because there is no order of operations
in a LISP (there are more than enough parentheses to clarify).

### Overlying Decisions

If you look in our examples/ directory, you can see the code samples we thought about to make
decisions about the structure of the language. We grounded on the following rules:
 - our generator is not going to be very interesting
   - we're not going to do any optimization in the first phase.
 - the syntax for the language must align closely with the information available in LLVM IR
   - there should usually be a one to one correspondence, or a very simple lookup in the tree.
 - names are more important than types, so types should follow the names.

### Divergence from Lisp

Our language looks like a LISP, but only for the easy parsing. We don't have any Lisp features
like homoiconicity, hygienic macros, garbage collection, etc. We also don't obey the traditional
LISP definition of a list. 

In LISP:

```
S-expression = List | Atom
List = ( Sexp* )
```

In Backbone:

```
S-expression = List | Atom
List = ( Word Sexp* )
Atom = Word | String | Char
```

### Future Plans

Verify the characters allowed in different expressions and allow for better error reporting for 
incorrect syntax. Validate the resulting tree and make sure it is a some minimal amount of 
a valid Backbone program instead of crashing or generating invalid LLVM. These were not 
prioritized because making the language Turing complete and capable of bootstrapping itself
were prioritized during the first phase.