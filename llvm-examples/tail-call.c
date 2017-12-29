#include <stdio.h>

void make0(unsigned int* i) {
  while (*i != 0) {
    (*i)++;
  }
  return;
}

void alsoMake0(unsigned int* i) {
  if (*i == 0) return;
  (*i)++;
  printf("*i:%u", *i);
  alsoMake0(i);
}

int main() {
  unsigned int a = 10000;
  make0(&a);
  printf("value: %d\n", a);
}

//TODO see if tail recursion optimization is applied by clang