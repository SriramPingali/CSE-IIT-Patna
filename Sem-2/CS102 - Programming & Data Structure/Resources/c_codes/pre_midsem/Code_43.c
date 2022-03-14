#include <stdio.h>
int main() {
  /* local variable definition */
  int a = 10;
/* do loop execution */
LOOP:
  do {
    if (a == 15) {
      /* skip the iteration */
      a = a + 1;
      goto LOOP;
    }
    printf("value of a: %d\n", a);
    a++;
  } while (a < 20);
  return 0;
}