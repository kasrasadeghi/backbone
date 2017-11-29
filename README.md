Working on the following versions:

Dependencies:
- cmake (3.9.6)
- make  (4.2.1)
- bash  (4.4.12)
- gcc   (7.2.0)
- clang (5.0.0)

Note: clang versions generate different LLVM IR, but should be roughly compatible. For example, clang 3.8 generates
names for arguments of functions and clang 5.0 leaves them unnamed, using %0... for them instead.