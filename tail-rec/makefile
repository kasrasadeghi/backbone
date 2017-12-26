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

exec\:%: %.ll
	clang -c $*.ll
	clang $*.o -o $*.exe
	-./$*.exe
	rm $*.o

build-v:
	@(cd ..; make)

build:
	(cd ..; make) &> /dev/null

parse\:%: %.bb build-v
	../backbone parse $*.bb

flatten\:%: %.bb build
	../backbone flatten $*.bb

gen\:%: build
	python3 test_gen.py $*

output\:%: build
	python3 test_output.py $*

help:
	@echo "make parse:hello  - parses hello.bb"
	@echo "make gen:hello    - generates hello.ll and checks it against hello.ok.ll"
	@echo "make output:hello - generates hello.ll, executes both hello.ll and hello.ok.ll and" \
							   "sees if they have the same output"

clean:
	find -name '*.ll' -not -name '*.ok.ll' | xargs rm
	rm *.exe
