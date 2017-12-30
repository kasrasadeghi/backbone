#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "blockify.h"
#include "flatten.h"
#include "qualify.h"
#include "gen.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    puts("Usage: backbone <parse|generateLLVM> <file>");
    return 1;
  }
  char* filename = argv[2];
  Sexp* program = parse(filename);
  if (strcmp(argv[1], "parse") == 0) {
    printSexp(program);
  }
  if (strcmp(argv[1], "flatten") == 0) {
    flatten(program);
    printSexp(program);
  }
  if (strcmp(argv[1], "qualify") == 0) {
    qualify(program);
    printSexp(program);
  }
  if (strcmp(argv[1], "blockify") == 0) {
    blockify(program);
    printSexp(program);
  }
  if (strcmp(argv[1], "gen") == 0) {
    flatten(program);
    generateLLVM(filename, program);
  }
  destroySexp(program);
}
