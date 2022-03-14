#include <stdio.h>
void main() {
  int n;
  float i = 1, pow = 2;
  printf("enter the value of n: ");
  scanf("%d", &n);
  for (i = 2; i <= n; i++) {
    pow = pow * 2;
  }
  printf("%f", pow);
}
