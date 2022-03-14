// Centigrade to Fahrenheit
#include <stdio.h>
void main() {
  float cent, fahr;

  printf("Enter Centigrate: ");
  scanf("%f", &cent);
  fahr = cent * (9.0 / 5.0) + 32;
  printf("%f C equals %f F\n", cent, fahr);
}
