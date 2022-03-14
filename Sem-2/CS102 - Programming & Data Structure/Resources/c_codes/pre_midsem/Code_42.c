// continue
#include <stdio.h>
int main() {
  /* local variable definition */
  int a = 10;
  /* do loop execution */
  do {
    if (a == 15) {
      /* skip the iteration */
      a = a + 1;
      continue;
    }
    printf("value of a: %d\n", a);
    a++;
  } while (a < 20);
  return 0;
}