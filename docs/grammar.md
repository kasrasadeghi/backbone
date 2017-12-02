```
Program ->
    | StrTable
    | Struct
    | Def
    
StrTable :=  str-table ( Int String )*
  
        
```

This is the grammar for a correct structure of S-expressions. Since the
S-expressions are already parsed into a tree, we need some way of 
defining the real structure of our tree and how we expect it to be.

There are several constructs for our grammar. Below they are explained:

 - words that begin with capital letters are Patterns or other NonTerminals
 
 - a Pattern must match some intuitive regex or some defined regex for a string
 
 - a word that begins with a lower case letter is a Word literal. 
   - It is the literal part of the tree that is represented by a Word, 
     typically by a Word found at the beginning of a list.
 
 - a line beginning with `;` (a semicolon) is a commented line.
   - typically for explaining extra rules for something to be semantically correct
   - these rules should be simple enough to check at parse-time
   
 - `(` (a left-paren) means the start of a List
 - `)` (a right-paren) means the end of a List
 - `)*` means the end of a List that is repeated in the parse-tree.