#include <stdio.h>
const int MAX = 3;
int main ()
{
int myarray[] = {10, 100, 200};
int i, *ptr[MAX];
for ( i = 0; i < MAX; i++)
{
ptr[i] = &myarray[i]; /* assign the address of integer. */
}
for ( i = 0; i < MAX; i++)
{
printf("Value of myarray[%d] = %d\n", i, *ptr[i] );
}
return 0;
}