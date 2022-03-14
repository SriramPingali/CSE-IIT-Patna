#include <stdio.h>
const int MAX = 3 ;
int main ()
{
int myarray[] = { 10 , 100 , 200 };
int i, *ptr;
/* let us have array address in pointer */
ptr = myarray;
for ( i = 0 ; i < MAX; i++)
{
printf("Address of myarray[%d] = %x\n", i, ptr );
printf("Value of myarray[%d] = %d\n", i, *ptr );
/* move to the next location */
ptr++;
}
return 0 ;
}