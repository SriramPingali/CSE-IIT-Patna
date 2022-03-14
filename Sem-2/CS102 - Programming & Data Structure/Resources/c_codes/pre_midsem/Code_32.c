// C Program to demonstrate the working of logical operators

#include <stdio.h>
int main() {
  int a = 5, b = 5, c = 10, result;

  result = (a == b) && (c > b);
  printf("(a == b) && (c > b) equals to %d \n", result);

  result = (a == b) && (c < b);
  printf("(a == b) && (c < b) equals to %d \n", result);

  result = (a == b) || (c < b);
  printf("(a == b) || (c < b) equals to %d \n", result);

  result = (a != b) || (c < b);
  printf("(a != b) || (c < b) equals to %d \n", result);

  result = !(a != b);
  printf("!(a == b) equals to %d \n", result);

  result = !(a == b);
  printf("!(a == b) equals to %d \n", result);

  return 0;
}