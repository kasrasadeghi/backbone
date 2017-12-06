

**Problem solved**

We are writing a lang with the intent of it being able to compile itself. 


**Choices made** 

Decided to use llvm; see notes on llvm (in git) for decisions 
Lots of syntax decisions (made by writing llvm first and then reverse-engineering what is necessary) (list significant ones)
Talk about how we did it (what is Sexp? etc)
Compile w clang and cmake - required to run our program.
Make sure it’s a tar.gz compiling on ubuntu
Thought about testing with google test (gtest) but ended up removing it so there are less dependencies. (can write own test suite)

Talk about new data strucutes - Sexp




**Challenges encountered**

Need to balance: how easy it is for us to compile code vs how easy it is to program in our language.
For example, deciding whether we need specific keywords or types everywhere.
(def main ((argc i32) (argv i8**)) i32 
Vs:
(def main (paramlist (param argc i32) (param argv i8**)) i32
Talk about how we made such decisions / considerations taken.




**Results**

Operations sematics or somethign here?
What did we accomplish at the end?
How far we got

Include: sample program -> parse tree -> llvm. 




