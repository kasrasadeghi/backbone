
//TODO finish

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
language is token by token, in-order (most of the time).