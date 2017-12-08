#include <stdio.h>
#include <stdlib.h>

typedef struct Sexp {
  char* value;
  struct Sexp** list;
  size_t length;
  size_t cap;
} Sexp;

Sexp* sexp(char* value) {
  Sexp* result = calloc(1, sizeof(Sexp));
  result->value = value;
  result->list = calloc(2, sizeof(Sexp*));
  result->length = 0;
  result->cap = 2;
  return result;
};

void printSexp(Sexp* s, int l) {
  for (int i = 0; i < l; ++i) {
    printf("  ");
  }
  printf("%s\n", s->value);
  for (int i = 0; i < s->length; ++i) {
    printSexp(s->list[i], l + 1);
  }
}

void pushSexp(Sexp* s, Sexp* child) {
  s->list[s->length] = child;
  ++s->length;
  if (s->length == s->cap) {
    s->cap *= 2;
    s->list = realloc(s->list, s->cap * sizeof(Sexp*));
  }
}
