IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.PRECIOUS: %.ll
%.o: %.ll
	clang -c $*.ll

run\:%:
	@clang -c $*.ll
	@gcc $*.o -o $*.exe
	@-./$*.exe
	@rm $*.exe $*.o
