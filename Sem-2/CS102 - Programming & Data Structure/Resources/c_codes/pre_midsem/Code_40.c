#include <stdio.h>

void main() {
  int a, i;
  a = 5;
  i = 1;
  do {
    printf("%d\t", a * i);
    i++;
  } while (i <= 10);
}