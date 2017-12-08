#include <stdlib.h>
#include <stdio.h>

typedef struct Basic {
  int a;
  long b;
  char c;
} Basic;

Basic* makeBasic(int a, long b, char c) {
  Basic* r = calloc(1, sizeof(Basic));
  r->a = a;
  r->b = b;
  r->c = c;
  return r;
}

int main() {
  Basic* t = makeBasic(1, 2, 'c');
  printf("Basic{%d, %ld, %c}", t->a, t->b, t->c);
}