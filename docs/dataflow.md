Compilers are really about dataflow, defined as a series of transformation to the data.

The over-arching compiler has several steps:

1. Parse a file into a Sexp representing the program structure.
2. Flatten the program structure so that there is one expression evaluation per statement.
3. Generate LLVM by recursively traversing the program structure.

These are shown in 3 different functions with the following signatures:

```c
Sexp* parse(char* filename);
void flatten(Sexp* program); 
void generateLLVM(char* filename, Sexp* program);
//Note: flatten should be Sexp* flatten(Sexp*), but for efficiency it modifies in place. 
```

`parse()` is gone over in the parser section of the report.

`flatten()` is gone over in the flatten section of the report.

`generateLLVM()` is gone over in the generation section of the report. 
