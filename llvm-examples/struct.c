#include <stdlib.h>
#include <stdio.h>

typedef struct Basic {
  int a;
} Basic;

Basic* makeBasic(int a) {
  Basic* r = calloc(1, sizeof(Basic));
  r->a = a;
  return r;
}

int main() {
  Basic* t = makeBasic(1);
  printf("Basic{%d}", t->a);
}