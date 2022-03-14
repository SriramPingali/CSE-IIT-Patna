#include <stdio.h>

void main() {
  int i, j;
  /* first for loop */
  for (i = 0; i < 5; i++) {
    printf("\n");
    /* second for loop inside the first */
    for (j = 0; j < 3; j++) {
      printf("\n%d", j);
    }
  }
}