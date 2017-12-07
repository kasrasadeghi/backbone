**Problem solved**

We are writing a lang with the intent of it being able to compile itself. 


**Choices made** 

Decided to use llvm; see notes on llvm (in git) for decisions 

https://llvm.org/:
The LLVM Project is a collection of modular and reusable compiler and toolchain technologies. The name "LLVM" itself is not an acronym.
began as research prpject at U of illinois, intended to support static and dynamic compilation of arbitrary programming langs. 
robust, flexible, reusable, versatile

nice for us because this means we can crate our own lang and llvm should be able to support its needs

clang: llvm native c, c++ compiler, aims for quick compilation and great error messages.

cmake: very flexible, allows you to easily build source code. uses a cmake list with many variables we set to tell it what to compile. very convienent and easy







Talk about how we did it (what is Sexp? etc)
Compile w clang and cmake - required to run our program.
Make sure it’s a tar.gz compiling on ubuntu
Thought about testing with google test (gtest) but ended up removing it so there are less dependencies. (can write own test suite)

Talk about new data strucutes - Sexp





**Results**

Operations sematics or somethign here?
What did we accomplish at the end?
How far we got

Include: sample program -> parse tree -> llvm. 

