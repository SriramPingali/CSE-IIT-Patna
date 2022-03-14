#include <stdio.h>
const int MAX = 3;
int main ()
{
int myarray[] = {10, 100, 200};
int i;
for (i = 0; i < MAX; i++)
{
printf("Value of myarray[%d] = %d\n", i, myarray[i] );
}
return 0;
}