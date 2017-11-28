FILES=$(wildcard *.c)
RESULTS=$(patsubst %.c,%.ll,$(FILES))
IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.DEFAULT: translate
translate: clean $(RESULTS)

%.ll: %.c
	clang $(IR_FLAGS) $*.c

.PRECIOUS: %.ll
%.o: %.ll
	clang -c $*.ll

.PHONY: clean
clean:
	rm -f *.ll

print:
	@echo $(RESULTS)

run\:%:
	@clang -c $*.ll
	@gcc $*.o -o $*.exe
	@-./$*.exe
	@rm $*.exe $*.o