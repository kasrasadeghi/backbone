The parser transforms an input file to a tree like structure according to the following CFG:

```
Program -> Sexp*
 
Sexp -> List | Atom

List -> ( Word Sexp* )

Atom -> String | Char | Word

//TODO regexes for String, Char
//TODO inclusion of whitespace, maybe in a second CFG to maintain the simplicity of this one
```


