IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.PRECIOUS: %.ll
%.o: %.ll
	clang -c $*.ll

# to run %.ll, compile it and run it
run\:%: %.ll
	@clang -c $*.ll
	@gcc $*.o -o $*.exe
	@-./$*.exe
	@rm $*.exe $*.o

build-v:
	@(cd ..; make)

build:
	(cd ..; make) &> /dev/null

test\:%: build
	python3 test_llvm.py hello

parse\:%: %.bb build-v
	../cmake-build-debug/backbone parse $*.bb

help:
	@echo "make test:hello  - generates hello.ll and checks it against hello.ok.ll"
	@echo "make parse:hello - parses hello.bb"
