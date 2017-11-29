#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "gen_llvm.h"

void gStrTable(FILE* file, Sexp* s) {
  //TODO
}

void gStruct(FILE* file, Sexp* s) {
  //TODO
}

void gDef(FILE* file, Sexp* s) {
  //TODO
}

void gProgram(FILE* file, Sexp* s) {
  for (int i = 0; i < s->length; ++i) {
    Sexp* child = s->list[i];
    if (strcmp(child->value, "str-table") == 0) {
      gStrTable(file, child);
    }
    if (strcmp(child->value, "struct") == 0) {
      gStruct(file, child);
    }
    if (strcmp(child->value, "def") == 0) {
      gDef(file, child);
    }
  }
}

void generateLLVM(char* filename, Sexp* sexp) {

  system("mkdir -p output &> /dev/null");// or something
  chdir("output");

  /* NOTE!!: don't uncomment below code as is. we have to strcat the filename with output/ or chdir
   * into output
   */
  remove(filename);
  FILE* result = fopen(filename, "a+");
  gProgram(result, sexp);
}