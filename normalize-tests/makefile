IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.PRECIOUS: %.ll
%.o: %.ll
	clang -c $*.ll

# to run %.ll, compile it and run it
from\:%: %.ll
	clang -c $*.ll
	clang $*.o -o $*.exe
	-./$*.exe
	rm $*.exe $*.o

build-v:
	@(cd ..; make)

build:
	(cd ..; make) &> /dev/null

parse\:%: %.bb build-v
	../backbone parse $*.bb

flatten\:%: %.bb build
	../backbone flatten $*.bb

test: build
	python3 test.py

help:
	@echo "make parse:hello   - parses hello.bb"
	@echo "make flatten:hello - flattens hello.bb"
