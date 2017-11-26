#include <stdio.h>
#include "parser.h"

void generateLLVM(Sexp* sexp) {
  //TODO actually finish
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: backbone parse <file>\n");
    return 1;
  }
  char* filename = argv[2];
  Sexp* program = parse(filename);
  printSexp(program, 0);
  generateLLVM(program);
  destroySexp(program);
}
