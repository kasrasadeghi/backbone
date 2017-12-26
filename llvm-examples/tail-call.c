#include <stdio.h>

void make0(int*);

int main() {
  int a = 10000;
  make0(&a);
  printf("value: %d\n", a);
}

//TODO see if tail recursion optimization is applied by clang