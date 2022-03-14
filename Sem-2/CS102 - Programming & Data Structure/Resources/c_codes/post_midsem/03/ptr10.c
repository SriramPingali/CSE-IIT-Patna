#include <stdio.h>
void disp( char ch)
{
   printf("%c ", ch);
}
int main()
{
   char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
   for (int x=0; x<10; x++)
   {
       /* I’m passing each element one by one using subscript*/
       disp (arr[x]);
   }

   return 0;
}