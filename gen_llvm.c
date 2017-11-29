#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "gen_llvm.h"


/**
 * Calculates the length of the given string, counting escaped characters only once.
 * The string must be well formed. (Doesn't end in \, backslashes escaped, etc.)
 * \<any character> --> 1 character
 */
size_t atomStrLen(char* s) {
  size_t len = 0;
  while (*s) {
    if (*s == '\\') {
        s++;
        if (*s < '0' || *s > '9' || *(s+1) < '0' || *(s+1) > '9') {
            // assertion failed.
            puts("*** Fatal: Backslash not followed by two digits.");
        }
        s++;
    }
    s++; len++;
  }
  return len;
}

void gStrTable(FILE* file, Sexp* s) {
  for (int i = 0; i < s->length; ++i) {
    char* string_entry = s->list[i]->list[0]->value;
    printf("@str.%d = private unnamed_addr constant [%lu x i8] c%s, align 1\n",
           i, atomStrLen(string_entry), string_entry);
  }
}

/**
 * Takes as input a type name and writes the qualified LLVM type to the output file
 * Precondition: type satisfies [a-zA-Z0-9]+[\*]*
 */
void gQualified(char* type) {
  char *primitive_types[] = {"i1", "i8", "i32", "i64", "u1", "u8", "u32", "u64"};

  for (int i = 0; i < 8; i++) {
    char *prim = primitive_types[i];
    size_t prim_len = strlen(prim);
    if (strncmp(type, prim, prim_len) == 0) {
      if (type + prim_len || *(type + prim_len) == '*') {
        // primitive type
        printf("i%s ", type + 1);
        return;
      }
    }
  } 
  printf("%%struct.%s", type);
}

void gStruct(FILE* file, Sexp* s) {
  printf("%%struct.%s = type { ", s->value);
  for (int i = 1; i < s->length; ++i) {
    char* type = s->list[i]->list[0]->value;
    gQualified(type);
    if (i != s->length - 1) printf(", ");
  }
  printf(" }\n");
}

void gDef(FILE* file, Sexp* s) {
  //TODO
}

void gProgram(FILE* file, Sexp* s) {
  printf("; ============= generate program ====================== ; \n");
  printf("; ModuleID = %s\n", s->value);
  printf("target datalayout = \"e-m:e-i64:64-f80:128-n8:16:32:64-S128\""
      "\ntarget triple = \"x86_64-unknown-linux-gnu\"\n");
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
