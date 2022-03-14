// C String function – strncpy
// char *strncpy( char *str1, char *str2, size_t n)
// size_t is unassigned short and n is a number.
// Case1: If length of str2 > n then it just copies first n characters of str2 into str1.
// Case2: If length of str2 < n then it copies all the characters of str2 into str1 and appends several terminator chars(‘\0’) to accumulate the length of str1 to make it n.

// Example of strncpy:

#include <stdio.h>
#include <string.h>
int main()
{
     char s1[] = "string 1";
     char s2[] = "string 2: I’m using strncpy now";
     /* this function has copied first 10 chars of s2 into s1*/
     strncpy(s1,s2, 12);
     printf("String s1 is: %s", s1);
     return 0;
} 