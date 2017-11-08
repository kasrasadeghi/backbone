#include <stdlib.h>
#include <stdio.h>
#include "str.h"
#include <ctype.h>

/**
 * An S-expression is either a list or an atom.
 * An atom is a char*
 * A list is a list of Sexps.
 */
typedef struct __Sexp__ {
  char* value;
  struct __Sexp__** list; // an array of pointers to Sexps
  size_t length;
  size_t capacity;
} Sexp;

void sexp_push(Sexp* l, Sexp* child) {
  l->list[l->length] = child;
  ++l->length;
  if (l->length == l->capacity) {
    printf("resizing %lu\n", l->capacity);
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

Sexp* pSexp(FILE*);

void pList(Sexp* curr, FILE* file) {
  puts("parse list");

  if (getc(file) != '(') {
    fprintf(stderr, "expecting lParen while parsing list");
    perror("backbone");
    exit(1);
  }

  curr->capacity = 5;
  curr->length = 0;
  curr->list = (Sexp**) calloc(curr->capacity, sizeof(Sexp*));

  int c;
  while (1) {
    c = peekc(file);
    if (c == ')' || c == EOF) break;
    sexp_push(curr, pSexp(file));
  }

  printf("%c: end of list\n", getc(file));
}

void pAtom(Sexp* curr, FILE* file) {
  puts("parse atom");
  // ignore whitespace

  String* str = str_make();

  int c;
  while (1) {
    c = getc(file);
    if (isspace(c) || c == ')') break;
    str_push(str, (char)c);
  }

  ungetc(c, file);

  curr->value = str_flush(str);

  free(str);
}

Sexp* pSexp(FILE* file) {
  Sexp* curr = (Sexp*) calloc(1, sizeof(Sexp));

  while (isspace(peekc(file))) {
    getc(file);
  }

  int c = peekc(file);
  switch (c) {
  case '(':
    printf("%c: ", c);
    pList(curr, file);
    return curr;
  default:
    printf("%c: ", c);
    pAtom(curr, file);
    return curr;
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
  // open a file
  FILE* file = fopen("../examples/main.kl", "r");

  // parse into tree
  Sexp* root = pProgram(file);

  printf("\n%lu\n", root->length);

  for (size_t i = 0; i < root->length; ++i) {
    sexp_print(root->list[i]);
    puts("");
  }
  puts("");
  return 0;
}
