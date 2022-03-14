

#include <stdio.h>
void main() {
  int x, y;
  printf("enter the value of x:");
  scanf("%d", &x);
  printf("enter the value of y:");
  scanf("%d", &y);
  if (x > y) {
    printf("x is greater than y\n");
  }
  if (x < y) {
    printf("x is less than y\n");
  }
  if (x == y) {
    printf("x is equal to y\n");
  }
  printf("End of Program");
}
