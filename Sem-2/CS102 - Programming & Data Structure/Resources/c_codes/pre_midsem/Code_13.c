

#include <stdio.h>
int main() // temp3.c
{
  int n = 4;
  float a = 2.5;
  char c = 'a'; // ASCII value 97
  printf("%d*%f = %f\n", n, a, n * a);
  printf("%d*%f+%c = %f\n", n, a, c, n * a + c);
  return 0;
}
