SOURCES=main.c str.c
OBJECTS=$(patsubt %.c,%,$(SOURCES)
IR_FLAGS=-emit-llvm -S -O3 -mllvm -inline-threshold=0

.DEFAULT: backbone
backbone: main.o str.o
	gcc main.o str.o -o $@
	./backbone

main.ll:
	clang $(IR_FLAGS) ../main.c

str.ll:
	clang $(IR_FLAGS) ../str.c

main.o: main.ll
	clang -c main.ll

str.o: str.ll
	clang -c str.ll

.PHONY: clean
clean:
	rm -f *.o *.ll
	rm -f backbone

again: clean backbone

end:
	rm -f backbone
	(make backbone)