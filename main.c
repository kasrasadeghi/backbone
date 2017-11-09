#include <stdlib.h>
#include <stdio.h>
#include "str.h"
#include <ctype.h>

/**
 * An S-expression is either a list or an atom.
 * An atom is a char*
 * A list is a list of Sexps.
 */
typedef struct Sexp {
  char* value;
  struct Sexp** list; // an array of pointers to Sexps
  size_t length;
  size_t capacity;
} Sexp;

void sexp_push(Sexp* l, Sexp* child) {
  l->list[l->length] = child;
  ++l->length;
  if (l->length == l->capacity) {
    l->capacity *= 2;
    l->list = (Sexp**)realloc(l->list, l->capacity);
  }
}

void sexp_print(Sexp* s) {
  if (s->list) {
    printf("(");
    for (size_t i = 0; i < s->length; ++i) {
      sexp_print(s->list[i]);
      if (i != s->length - 1) {
        putchar(' ');
      }
    }
    printf(")");
  } else {
    printf("%s", s->value);
  }
}

int peekc(FILE* file) {
  int c = getc(file);
  if (c != EOF) ungetc(c, file);
  return c;
}

int LINE = 1;
int getch(FILE* file) {
  int c = getc(file);
  if (c == '\n') ++LINE;
  return c;
}

int error(const char* msg) {
  fprintf(stderr, "%s", msg);
  perror("backbone");
  exit(1);
}

Sexp* pSexp(FILE*);

Sexp* pList(FILE* file) {
  puts("parse list");

  if (getch(file) != '(') {
    error("expecting lParen while parsing list");
  }
  
  Sexp* curr = (Sexp*) calloc(1, sizeof(Sexp));
  curr->capacity = 5;
  curr->length = 0;
  curr->list = (Sexp**) calloc(curr->capacity, sizeof(Sexp*));

  while (peekc(file) != ')') {
    if (peekc(file) == EOF) error("unmatched parenthesis");
    sexp_push(curr, pSexp(file));
  }

  printf("%c: end of list\n", getch(file));

  return curr;
}

Sexp* pAtom(FILE* file) {
  printf("parse atom %d\n", LINE);

  String* str = str_make();

  // parse string literals
  /*
  if (peekc(file) == '"') {
    str_push(str, (char)getch(file));

    while (peekc(file) != '"') {
      str_push(str, (char)getch(file));
    }
    str_push(str, (char)getch(file));
  }
  */

  while (!isspace(peekc(file)) && peekc(file) != ')') {
    str_push(str, (char)getch(file));
  }

  Sexp* curr = (Sexp*) calloc(1, sizeof(Sexp));
  curr->value = str_flush(str);
  free(str);
  
  return curr;
}

Sexp* pSexp(FILE* file) {
  while (isspace(peekc(file))) {
    getch(file);
  }

  printf("%c: ", peekc(file));
  if (peekc(file) == '(') {
    return pList(file);
  } else {
    return pAtom(file);
  }
}

Sexp* pProgram(FILE* file) {
  Sexp* program = (Sexp*) calloc(1, sizeof(Sexp));
  program->capacity = 5;
  program->length = 0;
  program->list = (Sexp**) calloc(program->capacity, sizeof(Sexp*));
  
  while (peekc(file) != EOF) {
    sexp_push(program, pSexp(file));
  }
  return program;
}

int main() {
  Sexp* root = pProgram(fopen("../examples/str.kl", "r"));

  printf("\n%lu\n", root->length);

  for (size_t i = 0; i < root->length; ++i) {
    sexp_print(root->list[i]);
    putchar('\n');
  }
  return 0;
}
