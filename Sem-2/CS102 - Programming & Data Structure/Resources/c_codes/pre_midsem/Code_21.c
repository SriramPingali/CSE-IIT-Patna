

#include <stdio.h>
void main() {
  int age;
  printf("Enter your age:");
  scanf("%d", &age);
  if (age >= 18) {
    /* This statement will only execute if the
     * above condition (age>=18) returns true
     */
    printf("You are eligible for voting");
  } else {
    /* This statement will only execute if the
     * condition specified in the "if" returns false.
     */
    printf("You are not eligible for voting");
  }
}
