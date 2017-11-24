SOURCES=main.c str.c
OBJECTS=$(patsubt %.c,%,$(SOURCES)

.DEFAULT: backbone
backbone: main.o str.o
	gcc main.o str.o -o $@
	./backbone

main.ll:
	clang -emit-llvm -S ../main.c

str.ll:
	clang -emit-llvm -S ../str.c

main.o: main.ll
	clang -c main.ll

str.o: str.ll
	clang -c str.ll

clean:
	rm -f *.o *.ll
