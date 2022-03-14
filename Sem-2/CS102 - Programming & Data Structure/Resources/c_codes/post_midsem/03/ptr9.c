#include <stdio.h>
int main ()
{
int temp;
int *ptr;
int **pptr;
temp = 3000;

/* take the address of temp */
ptr = &temp;
/* take the address of ptr using address of operator & */
pptr = &ptr;
/* take the value using pptr */
printf("Value of temp = %d\n", temp );
printf("Value available at *ptr = %d\n", *ptr );
printf("Value available at **pptr = %d\n", **pptr);
return 0;
}