FILES=$(wildcard *.c)
RESULTS=$(patsubst %.c,%.ll,$(FILES))
IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.DEFAULT: translate
translate: $(RESULTS)

%.ll: %.c
	clang $(IR_FLAGS) $*.c
	python3 clarify.py $*.ll

.PRECIOUS: %.ll
%.o: %.ll
	clang -c $*.ll

print:
	@echo $(RESULTS)

from\:%:
	@clang -c $*.ll
	@gcc $*.o -o $*.exe
	@-./$*.exe
	@rm $*.exe $*.o

help:
	@echo "translate - translate all files from .c to .ll"
	@echo "hello.ll  - translate from hello.c to hello.ll"
	@echo "run:hello - execute hello.ll"

clean:
	-find -name '*.ll' -not -name '*.ok.ll' -not -name '*.keep.ll' | xargs rm -f
	rm -f *.o