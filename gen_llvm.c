#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include "gen_llvm.h"

/**
 * Calculates the length of the given string, counting escaped characters only once.
 * The string must be well formed:
 *  - Doesn't end in \
 *  - backslashes escaped
 *
 * Escaped Characters: \xx
 *  - x stands for any capital hexadecimal digit.
 */
size_t atomStrLen(char* s) {
  size_t len = 0;
  for (;*s; s++, len++) {
    if (*s == '\\') {
      int counter = 0;
      s++; counter += isxdigit(*s) != 0;
      s++; counter += isxdigit(*s) != 0;
      if (counter != 2) {
        fprintf(stderr, "backbone: Backslash not followed by two hexadecimal digits.");
        exit(EXIT_FAILURE);
      }
    }
  }
  return len;
}

static size_t* _str_table;
static size_t _str_table_len;

void gStrTable(Sexp* s) {
  _str_table_len = s->length;
  _str_table = calloc(_str_table_len, sizeof(long));
  for (int i = 0; i < s->length; ++i) {
    char* string_entry = s->list[i]->list[0]->value;
    size_t len = atomStrLen(string_entry) - 2;
    printf("@str.%d = private unnamed_addr constant [%lu x i8] c%s, align 1\n",
           i, len, string_entry);
    _str_table[i] = len;
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
        printf("i%s", type + 1);
        return;
      }
    }
  }
  printf("%%struct.%s", type);
}

void gStruct(Sexp* s) {
  printf("%%struct.%s = type { ", s->value);
  for (int i = 1; i < s->length; ++i) {
    char* type = s->list[i]->list[0]->value;
    gQualified(type);
    if (i != s->length - 1) printf(", ");
  }
  printf(" }\n");
}

void gExpr(Sexp*);

void gCall(Sexp* s) {
  printf("call ");
  gQualified(s->list[2]->value);
  printf(" (");
  Sexp* types = s->list[1];
  for (int i = 0; i < types->length; ++i) {
    gQualified(types->list[i]->value);
    if (i != types->length - 1) printf(", ");
  }
  printf(") ");

  printf("@%s(", s->list[0]->value);
  Sexp* args = s->list[3];
  for (int i = 0; i < types->length; ++i) {
    gQualified(types->list[i]->value);
    printf(" ");
    gExpr(args->list[0]);
  }
  printf(")");
}

void gStrGet(Sexp* s) {
  size_t index = strtoul(s->list[0]->value, NULL, 10);
  size_t len = _str_table[index];
  printf("getelementptr inbounds ([%lu x i8], [%lu x i8]* @str.%lu, i64 0, i64 0)",
         len, len, index);
}

void gExpr(Sexp* s) {
  if (strcmp(s->value, "call") == 0) {
    gCall(s);
  }
  if (strcmp(s->value, "str-get") == 0) {
    gStrGet(s);
  }
}

void gLet(Sexp* l) {
  printf("  %%%s = ", l->list[0]->value);
  gExpr(l->list[1]);
}

void gReturn(Sexp* s) {
  printf("  ret %s %s", s->list[1]->value, s->list[0]->value);
}

void gStmt(Sexp* s) {
  if (strcmp(s->value, "let") == 0) {
    gLet(s);
  }
  if (strcmp(s->value, "return") == 0) {
    gReturn(s);
  }
  printf("\n");
}

void gDef(Sexp* s) {
  printf("define ");
  gQualified(s->list[2]->value);
  printf(" @%s", s->list[0]->value);

  printf("(");
  //TODO parameters
  printf(") ");

  printf("{\n");
  printf("entry:\n");
  for (int i = 3; i < s->length; ++i) {
    gStmt(s->list[i]);
  }
  printf("}\n");
}

void gDecl(Sexp* s) {
  printf("declare ");
  gQualified(s->list[2]->value);

  printf(" @%s", s->list[0]->value);

  Sexp* typelist = s->list[1];
  printf("(");
  for (int i = 0; i < typelist->length; ++i) {
    gQualified(typelist->list[i]->value);
    if (i != typelist->length - 1) printf(", ");
  }

  printf(")");

  printf("\n");
}

void gProgram(Sexp* s) {
  printf("; ModuleID = '%s'\n", s->value);
  printf("target datalayout = \"e-m:e-i64:64-f80:128-n8:16:32:64-S128\""
      "\ntarget triple = \"x86_64-unknown-linux-gnu\"\n");

  for (int i = 0; i < s->length; ++i) {
    puts("");
    Sexp* child = s->list[i];
    if (strcmp(child->value, "str-table") == 0) {
      gStrTable(child);
    }
    if (strcmp(child->value, "struct") == 0) {
      gStruct(child);
    }
    if (strcmp(child->value, "def") == 0) {
      gDef(child);
    }
    if (strcmp(child->value, "decl") == 0) {
      gDecl(child);
    }
  }
}

/**
 * Assumes filename is of the form %.bb
 * Creates an output filename in the form %.ll
 */
void redirect_output(char* filename) {
  size_t namelen = strlen(filename) + 1;
  char out[namelen];
  strncpy(out, filename, namelen);
  out[namelen - 2] = 'l';
  out[namelen - 3] = 'l';
  printf("default output: %s\n", out);
  if (access(out, F_OK) != -1) {
    remove(out);
  }
  close(STDOUT_FILENO);
  int fd = open(out, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
  if (fd != STDOUT_FILENO) {
    fprintf(stderr, "backbone: not directory to %s for some reason", out);
  }
}

void generateLLVM(char* filename, Sexp* sexp) {
  redirect_output(filename);
  gProgram(sexp);
}
