#include <stdio.h>
void main() {
  float x, y;
  int d1, d2 = 10;
  printf("Please enter 2 floats and 1 int:  ");
  scanf("%f%f%d", &x, &y, &d1);
  printf("%f plus %f is %f\n", x, y, x + y);
  printf("%d minus %d is %d\n", d1, d2, d1 - d2);
  // printf( "%f minus %f is %f\n", d1, d2, d1-d2);
}
