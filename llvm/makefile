SOURCES=$(wildcard ../src/*.c)
IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.PHONY: sources
sources:
	(cd build-dir; rm -f *.c; rm -f *.h)
	cp ../src/*.c build-dir
	cp ../src/*.h build-dir
