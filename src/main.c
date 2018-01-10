#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "normalize.h"
#include "qualify.h"
#include "gen.h"
#include "blockify.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    puts("Usage: backbone <parse|normalize|qualify|gen> <file>");
    return 1;
  }
  char* filename = argv[2];
  Sexp* program = parse(filename);
  if (strcmp(argv[1], "parse") == 0) {
    printSexp(program);
  }
  if (strcmp(argv[1], "blockify") == 0) {
    blockify(program);
    printSexp(program);
  }
  if (strcmp(argv[1], "normalize") == 0) {
    blockify(program);
    normalize(program);
    printSexp(program);
  }
  if (strcmp(argv[1], "qualify") == 0) {
    qualify(program);
    printSexp(program);
  }
  if (strcmp(argv[1], "gen") == 0) {
    normalize(program);
    qualify(program);
    generateLLVM(filename, program);
  }
  destroySexp(program);
}
