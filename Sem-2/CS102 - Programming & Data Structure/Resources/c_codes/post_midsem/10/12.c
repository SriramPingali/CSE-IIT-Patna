// Example of strncat:

#include <stdio.h>
#include <string.h>
int main()
{
     char s1[10] = "Hello";
     char s2[10] = "World";
     strncat(s1,s2, 3);
     printf("Concatenation using strncat: %s", s1);
     return 0;
}