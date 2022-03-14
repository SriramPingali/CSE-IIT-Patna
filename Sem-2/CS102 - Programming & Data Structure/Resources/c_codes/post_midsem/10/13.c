// Example of strcpy:

#include <stdio.h>
#include <string.h>
int main()
{
     char s1[30] = "string 1";
     char s2[30] = "string 2 : I’m gonna copied into s1";
     /* this function has copied s2 into s1*/
     strcpy(s1,s2);
     printf("String s1 is: %s", s1);
     return 0;
}
