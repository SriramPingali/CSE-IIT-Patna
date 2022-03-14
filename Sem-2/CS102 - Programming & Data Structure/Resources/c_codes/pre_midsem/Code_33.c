// C Program to demonstrate the working of arithmetic operators
#include <stdio.h>
int main() {
  int a = 5, b = 5, c = 10;

  printf("%d == %d = %d \n", a, b, a == b); // true
  printf("%d == %d = %d \n", a, c, a == c); // false

  printf("%d > %d = %d \n", a, b, a > b); // false
  printf("%d > %d = %d \n", a, c, a > c); // false

  printf("%d < %d = %d \n", a, b, a < b); // false
  printf("%d < %d = %d \n", a, c, a < c); // true

  printf("%d != %d = %d \n", a, b, a != b); // false
  printf("%d != %d = %d \n", a, c, a != c); // true

  printf("%d >= %d = %d \n", a, b, a >= b); // true
  printf("%d >= %d = %d \n", a, c, a >= c); // false

  printf("%d <= %d = %d \n", a, b, a <= b); // true
  printf("%d <= %d = %d \n", a, c, a <= c); // true

  return 0;
}