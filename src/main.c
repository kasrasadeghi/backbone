#include <stdio.h>
#include <string.h>
#include <zconf.h>
#include "parser.h"
#include "qualify.h"
#include "blockify.h"
#include "normalize.h"
#include "flatten.h"
#include "gen.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    puts("Usage: backbone <parse|generateLLVM> <file>");
    return 1;
  }
  // parse
  // -> become    TODO
  // -> callStmt  TODO
  // -> qualify
  // -> blockify
  // -> normalize
  // -> flatten   TODO
  // -> gen

  char* filename = argv[2];
  Sexp* program = parse(filename);
  if (strcmp(argv[1], "parse") == 0) {
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
  if (strcmp(argv[1], "normalize") == 0) {
    blockify(program);
    normalize(program);
    printSexp(program);
    printf("hello\n");
    sleep(20);
  }
  if (strcmp(argv[1], "flatten") == 0) {
    flatten(program);
    printSexp(program);
  }
  if (strcmp(argv[1], "gen") == 0) {
    flatten(program);
    generateLLVM(filename, program);
  }
  destroySexp(program);
}
