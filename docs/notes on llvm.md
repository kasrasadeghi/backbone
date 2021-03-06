## Simplification

Where you see "tail call", you can replace it with "call". It's just an optimization.
 - investigate assembly

Maybe: Where you see "attributes #\d = { ... }" you can replace it with "{ nounwind }".
 - investigate assembly
 - see if that's true in all cases

LLVM doesn't even seem to need the attributes. We don't need to generate those.

## Notes

Return types and argument types in LLVM must match exactly. There is no implicit type-conversion.

LLVM doesn't store string literals as char*'s. They're constants somewhere in memory and then casted
 to char*'s when used.

The line we're going to draw for type inference is drawn from the following rule of thumb:
  If the LLVM IR needs the type, we're going to include it in the regular language.

On Clang-5 numbering!!!:
  Say you're defining a function. see print_int.ll for the example I'm talking about.
  Clang generates LLVM IR that starts counting from some number, according to the following rules:

  It starts at 0, as it should. This only counts the arguments (and the instructions/values) that are unnamed. If they are
  named they are skipped by the counter.

  If it has arguments, the arguments are counted.

  Once the arguments are finished, the instruction pointer to the start of
  the function is counted if it is not named.

  Then all of the unnamed values and labels are counted.
