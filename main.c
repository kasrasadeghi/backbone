#include <stdlib.h>
#include <stdio.h>
#include "str.h"

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

void sexp_push(Sexp* sexp, Sexp* child) {
  sexp->list[sexp->length] = child;
  ++sexp->length;
  if (sexp->length == sexp->capacity) {
    sexp->capacity *= 2;
    sexp->list = (Sexp**)realloc(sexp->list, sexp->capacity);
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

  curr->capacity = 2;
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
    if (c == ' ' || c == '\n' || c == '\r' || c == ')') break;
    str_push(str, (char)c);
  }

  ungetc(c, file);

  curr->value = str_flush(str);

  free(str);
}

Sexp* pSexp(FILE* file) {
  Sexp* curr = (Sexp*) calloc(1, sizeof(Sexp));

  int c;
ignore:
  c = peekc(file);
  printf("%c: ", c);
  switch (c) {
  case ' ':
  case '\n':
    getc(file);
    puts("ignore");
    goto ignore;
  case '(':
    pList(curr, file);
    return curr;
  default:
    pAtom(curr, file);
    return curr;
  }
}

int main() {
  // open a file
  FILE* file = fopen("../examples/hello.kl", "r");

  // parse into tree
  Sexp* root = pSexp(file);

  puts("");
  printf("%lu\n", root->length);

  sexp_print(root);
  return 0;
}