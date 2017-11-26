SOURCES=main.c str.c
OBJECTS=$(patsubt %.c,%,$(SOURCES)
IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.DEFAULT: backbone
backbone: main.o str.o
	gcc main.o str.o -o $@

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

end:
	rm -f backbone
	(make backbone)

run\:%: backbone
