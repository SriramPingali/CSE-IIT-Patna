
#include <stdio.h>

void main() {
  // The sum of these numbers will equal INT_MAX
  // If any of them is incremented by 1, overflow
  // will occur
  int num1 = 2147483647;

  printf("\n%d", num1);

  printf("\n%d", ++num1);
}
