#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include "gen.h"

//region static globals

static size_t* _str_table;     // keeps track of how long each entry is
static size_t _str_table_len;  // keeps track of the entry count

static size_t _if_counter = 0; // keeps track of the labels for if

static Sexp* _p = NULL;        // the program sexp

//endregion

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

/**
 * Globals:
 *  - _str_table
 *  - _str_table_len
 */
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
  if (strcmp(type, "void") == 0 || strcmp(type, "...") == 0) {
    printf("%s", type);
    return;
  }

  char *primitive_types[] = {"i1", "i8", "i32", "i64", "u1", "u8", "u32", "u64"};

  for (int i = 0; i < 8; i++) {
    char *prim = primitive_types[i];
    size_t prim_len = strlen(prim);
    if (strncmp(type, prim, prim_len) == 0) {
      if (*(type + prim_len) == 0 || *(type + prim_len) == '*') {
        // primitive type
        printf("i%s", type + 1);
        return;
      }
    }
  }
  printf("%%struct.%s", type);
}

void gStruct(Sexp* s) {
  printf("%%struct.%s = type { ", s->list[0]->value);
  for (int i = 1; i < s->length; ++i) {
    char* type = s->list[i]->list[0]->value;
    gQualified(type);
    if (i != s->length - 1) printf(", ");
  }
  printf(" }\n");
}

void gValue(Sexp*);

/**
 * Looks up the name of a declaration and returns a pointer to it.
 * Globals:
 *  - _p
 *
 * Assumes that all functions available for calling are children of _p.
 * Notes:
 *  - Won't work for multi-file projects
 *
 */
Sexp* lookupDecl(Sexp* name) {
  //TODO refactor this to lookupFunction that checks both decls and defs
  for (int i = 0; i < _p->length; ++i) {
    Sexp* child = _p->list[i];
    if (isDecl(child)) {
      if (strcmp(child->list[0]->value, name->value) == 0) {
        return child;
      }
    }
  }
  assert(0); // lookups should always be found
}

void gCallVargs(Sexp* s) {
  Sexp* original = lookupDecl(s->list[0]);
  printf("call ");
  gQualified(s->list[2]->value);
  printf(" (");

  Sexp* types = original->list[1];
  for (int i = 0; i < types->length; ++i) {
    gQualified(types->list[i]->value);
    if (i != types->length - 1) printf(", ");
  }
  printf(") ");

  printf("@%s(", s->list[0]->value);
  Sexp* args = s->list[3];
  Sexp* arg_types = s->list[1];
  assert(args->length == arg_types->length);
  for (int i = 0; i < arg_types->length; ++i) {
    gQualified(arg_types->list[i]->value);
    printf(" ");
    gValue(args->list[i]);
    if (i != arg_types->length - 1) {
      printf(", ");
    }
  }
  printf(")");
}

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
    gValue(args->list[i]);
    if (i != types->length - 1) {
      printf(", ");
    }
  }
  printf(")");
}

void gStrGet(Sexp* s) {
  size_t index = strtoul(s->list[0]->value, NULL, 10);
  size_t len = _str_table[index];
  printf("getelementptr inbounds ([%lu x i8], [%lu x i8]* @str.%lu, i64 0, i64 0)",
         len, len, index);
}

void gIcmp(Sexp* s) {
  printf("icmp ");
  if (! (strcmp(s->value, "=") == 0 || strcmp(s->value, "!=") == 0)) {
    if (s->list[0]->value[0] == 'i') {
      printf("s");
    } else if (s->list[0]->value[0] == 'u') {
      printf("u");
    } else {
      fprintf(stderr, "type for comparison does not start with u or i\n");
      exit(EXIT_FAILURE);
    }
  }

  if (strcmp(s->value, "<") == 0) {
    printf("lt");
  }
  else if (strcmp(s->value, "<=") == 0) {
    printf("le");
  }
  else if (strcmp(s->value, ">") == 0) {
    printf("gt");
  }
  else if (strcmp(s->value, ">=") == 0) {
    printf("ge");
  }
  else if (strcmp(s->value, "=") == 0) {
    printf("eq");
  }
  else if (strcmp(s->value, "!=") == 0) {
    printf("ne");
  }

  printf(" %s ", s->list[0]->value);

  gValue(s->list[1]);
  printf(", ");
  gValue(s->list[2]);
}

/* (load Type Value) */
void gLoad(Sexp* s) {
  printf("load ");
  gQualified(s->list[0]->value);
  printf(", ");
  gQualified(s->list[0]->value);
  //TODO check if below is valid. is below always flattened?
  printf("* ");
  printf("%%%s", s->list[1]->value);
}

/* (index PtrValue Type IntValue) */
/* requires Value to be of-type i32 or literal */
void gIndex(Sexp* s) {
  printf("getelementptr inbounds ");
  gQualified(s->list[1]->value);
  printf(", ");
  gQualified(s->list[1]->value);
  printf("*");
  //TODO check if below is valid. is below always flattened?
  printf(" %%%s, i32 0, i32 ", s->list[0]->value);
  gValue(s->list[2]);
}

/* (cast TypeTo TypeFrom PtrName) */
void gCast(Sexp* s) {
  printf("bitcast ");
  gQualified(s->list[0]->value);
  printf(" %%%s to ", s->list[2]->value);
  gQualified(s->list[1]->value);
}

/* (+ Type Value Value) */
void gAdd(Sexp* s) {
  printf("add %s ", s->list[0]->value);
  gValue(s->list[1]);
  printf(", ");
  gValue(s->list[2]);
  printf("\n");
}

void gMathBinop(Sexp* s) {
  if (isAdd(s)) {
    gAdd(s);
    return;
  }
  assert(0); // the only math binop is add
}

void gExpr(Sexp* s) {
  if (isCall(s)) {
    gCall(s);
  }
  else if (isCallVargs(s)) {
    gCallVargs(s);
  }
  else if (isMathBinop(s)) {
    gMathBinop(s);
  }
  else if (isIcmp(s)) {
    gIcmp(s);
  }
  else if (strcmp(s->value, "load") == 0) {
    gLoad(s);
  }
  else if (strcmp(s->value, "index") == 0) {
    gIndex(s);
  }
  else if (isCast(s)) {
    gCast(s);
  }
  else {
    gValue(s);
  }
}

void gLet(Sexp* l) {
  printf("  %%%s = ", l->list[0]->value);
  gExpr(l->list[1]);
}

/**
 * Values are fully evaluated expressions, like a name that has been set or a literal.
 * So far this function supports only names and integer literals.
 *
 * TODO: check for support for struct literals and other kinds of literals.
 *
 * Assumes that value names don't start with a digit.
 */
void gValue(Sexp* s) {
  if (strcmp(s->value, "true") == 0) {
    printf("1");
  }
  else if (strcmp(s->value, "false") == 0) {
    printf("0");
  }
  else if (strcmp(s->value, "str-get") == 0) {
    gStrGet(s);
  }
  else if (isdigit(s->value[0])) {
    printf("%s", s->value);
  }
  else {
    printf("%%%s", s->value);
  }
}

void gReturn(Sexp* s) {
  if (strcmp(s->list[0]->value, "void") == 0) {
    printf("  ret void");
    return;
  }

  printf("  ret ");
  gQualified(s->list[1]->value);
  printf(" ");
  gValue(s->list[0]);
}

void gStmt(Sexp* s);

/**
 * Globals:
 *  - _if_counter
 */
void gIf(Sexp* s) {
  size_t label = _if_counter++;

  printf("  br i1 ");
  gValue(s->list[0]);
  printf(", label %%then%lu, label %%post%lu\n",
         label, label);

  printf("then%lu:\n", label);
  /* statements */
  for (int i = 1; i < s->length; ++i) {
    gStmt(s->list[i]);
  }

  printf("  br label %%post%lu\n", label);

  printf("post%lu:", label);
}

/* (auto FreshName Type) */
void gAuto(Sexp* s) {
  printf("  %%%s = alloca ", s->list[0]->value);
  gQualified(s->list[1]->value);
}

/* (store Value Type PtrName) */
void gStore(Sexp* s) {
  char* type = s->list[1]->value;
  printf("  store ");

  gQualified(type);
  printf(" ");
  gValue(s->list[0]);

  printf(", ");
  gQualified(type);
  printf("*");
  printf(" %%%s", s->list[2]->value);
}

void gStmt(Sexp* s) {
  if (strcmp(s->value, "let") == 0) {
    gLet(s);
  }
  else if (strcmp(s->value, "return") == 0) {
    gReturn(s);
  }
  else if (strcmp(s->value, "if") == 0) {
    gIf(s);
  }
  else if (isCall(s) && strcmp(s->list[2]->value, "void") == 0) {
    printf("  ");
    gCall(s);
  }
  else if (isCallVargs(s) && strcmp(s->list[2]->value, "void") == 0) {
    printf("  ");
    gCallVargs(s);
  }
  else if (strcmp(s->value, "store") == 0) {
    gStore(s);
  }
  else if (isAuto(s)) {
    gAuto(s);
  }
  else {
    fprintf(stderr, "Error: unmatched statement!\n");
    printf("  UNMATCHED STATEMENT:\n");
    printSexp(s, 0);
  }
  printf("\n");
}

void gDef(Sexp* s) {
  _if_counter = 0;

  printf("define ");
  gQualified(s->list[2]->value);
  printf(" @%s", s->list[0]->value);

  printf("(");
  Sexp* params = s->list[1];
  for (int i = 0; i < params->length; ++i) {
    Sexp* param = params->list[i];
    gQualified(param->list[0]->value);
    printf(" %%%s", param->value);
    if (i != params->length - 1) {
      printf(", ");
    }
  }
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

  Sexp* types = s->list[1];
  printf("(");
  for (int i = 0; i < types->length; ++i) {
    gQualified(types->list[i]->value);
    if (i != types->length - 1) printf(", ");
  }

  printf(")");

  printf("\n");
}


void gProgram(Sexp* s) {
  _p = s;
  printf("; ModuleID = '%s'\n", s->value);
  printf("target datalayout = \"e-m:e-i64:64-f80:128-n8:16:32:64-S128\""
      "\ntarget triple = \"x86_64-unknown-linux-gnu\"\n");

  for (int i = 0; i < s->length; ++i) {
    printf("\n");
    Sexp* child = s->list[i];
    if (strcmp(child->value, "str-table") == 0) {
      gStrTable(child);
    }
    if (strcmp(child->value, "struct") == 0) {
      gStruct(child);
    }
    if (isDef(child)) {
      gDef(child);
    }
    if (isDecl(child)) {
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
  //TODO figure out how to make the line below work with the python scripts. for some reason it
  // shows up in the file when calling backbone in python. we might need to use call, not
  // check_output.
//  printf("default output: %s\n", out);
  if (access(out, F_OK) != -1) {
    remove(out);
  }
  close(STDOUT_FILENO);
  int fd = open(out, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
  if (fd != STDOUT_FILENO) {
    fprintf(stderr, "backbone: not replacing stdout for some reason\n");
    exit(EXIT_FAILURE);
  }
}

void generateLLVM(char* filename, Sexp* sexp) {
  redirect_output(filename);
  gProgram(sexp);
}
