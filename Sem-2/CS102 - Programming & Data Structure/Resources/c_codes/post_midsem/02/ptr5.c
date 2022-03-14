include <stdio.h>
const int MAX = 3 ;
int main ()
{
int myarray[] = { 10 , 100 , 200 };
int i, *ptr;
/* let us have address of the first element in pointer */
ptr = myarray;
i = 0 ;
while ( ptr <= &myarray[MAX - 1 ] )
{
printf("Address of myarray[%d] = %x\n", i, ptr );
printf("Value of myarray[%d] = %d\n", i, *ptr );
/* point to the previous location */
ptr++;
i++;
}
return 0 ;
}