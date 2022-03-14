// It compares both the string till n characters or in other words it compares first n characters of both the strings.

// Example of strncmp:

#include <stdio.h>
#include <string.h>
int main()
{
     char s1[20] = "BeginnersBook";
     char s2[20] = "BeginnersBook.COM";
     /* below it is comparing first 8 characters of s1 and s2*/
     if (strncmp(s1, s2, 8) ==0)
     {
         printf("string 1 and string 2 are equal");
     }else
     {
         printf("string 1 and 2 are different");
     }
     return 0;
}