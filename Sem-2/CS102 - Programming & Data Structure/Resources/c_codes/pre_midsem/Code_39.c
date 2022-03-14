#include <stdio.h>

void main() {
  int x;
  x = 1;
  while (x <= 10) {
    printf("%d\t", x);
    /* below statement means, do x = x+1, increment x by 1*/
    x++;
  }
}