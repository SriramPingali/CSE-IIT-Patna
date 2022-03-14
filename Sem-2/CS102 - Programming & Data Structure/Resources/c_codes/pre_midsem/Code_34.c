// C Program to demonstrate the working of assignment operators
#include <stdio.h>
int main() {
  int a = 5, c;

  c = a;
  printf("c = %d \n", c);

  c += a; // c = c+a
  printf("c = %d \n", c);

  c -= a; // c = c-a
  printf("c = %d \n", c);

  c *= a; // c = c*a
  printf("c = %d \n", c);

  c /= a; // c = c/a
  printf("c = %d \n", c);

  c %= a; // c = c%a
  printf("c = %d \n", c);

  return 0;
}