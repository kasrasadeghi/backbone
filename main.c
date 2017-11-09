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
    printf("resizing %lu to %lu\n", l->capacity, l->capacity*2);
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

int error(const char* msg) {
  fprintf(stderr, msg);
  perror("backbone");
  exit(1);
}

Sexp* pSexp(FILE*);

Sexp* pList(FILE* file) {
  puts("parse list");

  if (getc(file) != '(') {
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

  printf("%c: end of list\n", getc(file));

  return curr;
}

Sexp* pAtom(FILE* file) {
  puts("parse atom");

  String* str = str_make();

  while (!isspace(peekc(file)) && peekc(file) != ')') {
    str_push(str, (char)getc(file));
  }

  Sexp* curr = (Sexp*) calloc(1, sizeof(Sexp));
  curr->value = str_flush(str);
  free(str);
  
  return curr;
}

Sexp* pSexp(FILE* file) {
  while (isspace(peekc(file))) {
    getc(file);
  }

  int c = peekc(file);
  switch (c) {
  case '(':
    printf("%c: ", c);
    return pList(file);
  default:
    printf("%c: ", c);
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
  FILE* file = fopen("../examples/test.kl", "r");
  Sexp* root = pProgram(file);

  printf("\n%lu\n", root->length);

  for (size_t i = 0; i < root->length; ++i) {
    sexp_print(root->list[i]);
    putchar('\n');
  }
  return 0;
}
