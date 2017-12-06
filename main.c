#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "gen_llvm.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    puts("Usage: backbone <parse|generateLLVM> <file>");
    return 1;
  }
  if (strcmp(argv[1], "parse") == 0) {
    char* filename = argv[2];
    Sexp* program = parse(filename);
    printSexp(program, 0);
    destroySexp(program);
  }
  if (strcmp(argv[1], "flatten") == 0) {
    char* filename = argv[2];
    Sexp* program = parse(filename);
    flatten(program);
    printSexp(program, 0);
    destroySexp(program);
  }
  if (strcmp(argv[1], "generateLLVM") == 0) {
    char* filename = argv[2];
    Sexp* program = parse(filename);
    generateLLVM(filename, program);
    destroySexp(program);
  }
}
