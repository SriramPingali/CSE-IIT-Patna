#include <stdio.h>
const int MAX = 3 ;
int main ()
{
int myarray[] = { 10 , 100 , 200 };
int i, *ptr;
/* let us have array address in pointer */
ptr = &myarray[MAX- 1 ];
for ( i = MAX; i > 0 ; i--)
{
printf("Address of myarray[%d] = %x\n", i, ptr );
printf("Value of myarray[%d] = %d\n", i, *ptr );
/* move to the previous location */
ptr--;
}
return 0 ;
}