OBJECTS=main.o str.o parser.o
IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.DEFAULT: backbone
backbone: $(OBJECTS)
	gcc $(OBJECTS) -o $@

%.ll: ../%.c
	clang $(IR_FLAGS) ../$*.c

.PRECIOUS: %.ll
%.o: %.ll
	clang -c $*.ll


.PHONY: clean
clean:
	rm -f *.o *.ll
	rm -f backbone

again: clean backbone

from\:%.ll:
	clang -c $*.ll
	gcc $(OBJECTS) -o backbone
	./backbone parse ../parser-tests/main.bb

run\:%: backbone
