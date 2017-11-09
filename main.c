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
  fprintf(stderr, "%s\n", msg);
  perror("backbone");
  exit(1);
}

void pWhitespace(FILE* file) {
  while (isspace(peekc(file)) && peekc(file) != EOF) {
    getch(file);
  }
  if (peekc(file) == EOF) {
    error("ran into end of file while parsing whitespace");
  }
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
    Sexp* child = pSexp(file);
    sexp_push(curr, child);
  }

  printf("%c: end of list\n", getch(file));

  return curr;
}

Sexp* pAtom(FILE* file) {
  puts("parse atom");
  printf("LINE: %d\n", LINE);

  static String str = {NULL, 0, 10};
  if (!str.list) str.list = (char*)calloc(10, 1);

  // parse char literal
  if (peekc(file) == '\'') {
    str_push(&str, (char)getch(file));

    while (peekc(file) != '\'') {
      str_push(&str, (char)getch(file));
    }
    str_push(&str, (char)getch(file));
  }

  // parse string literal
  if (peekc(file) == '"') {
    str_push(&str, (char)getch(file));

    while (peekc(file) != '"') {
      str_push(&str, (char)getch(file));
    }
    str_push(&str, (char)getch(file));
  }

  else while (!isspace(peekc(file)) && peekc(file) != ')') {
    str_push(&str, (char)getch(file));
  }

  Sexp* curr = (Sexp*) calloc(1, sizeof(Sexp));
  curr->value = str_flush(&str);
  
  return curr;
}

Sexp* pSexp(FILE* file) {
  pWhitespace(file);

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
    pWhitespace(file);
  }
  return program;
}

int main() {
  Sexp* root = pProgram(fopen("../examples/sigabrt.kl", "r"));

  printf("\n%lu\n", root->length);

  for (size_t i = 0; i < root->length; ++i) {
    sexp_print(root->list[i]);
    putchar('\n');
  }
  return 0;
}
