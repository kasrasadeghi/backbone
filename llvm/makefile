SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(wildcard *.h)
IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.DEFAULT:backbone

# disabling implicit rules for gnu make
.SUFFIXES:

# llvm ir is precious
.PRECIOUS: %.ll

%.ll: %.c
	clang ${IR_FLAGS} $*.c

%.o: %.ll
	clang -c $*.ll

backbone: sources ${OBJECTS}
	clang ${OBJECTS} -o backbone
	(make clean)

.PHONY: sources
sources:
	cp ../src/*.c .
	cp ../src/*.h .

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.h.gch

.PHONY: clear
clear: clean
	rm -f *.c
	rm -f *.h