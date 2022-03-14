// Example 2: gets() and puts()
#include <stdio.h>
int main()
{
    char name[30];
    printf("Enter name: ");
    gets(name);     // read string
    printf("Name: ");
    puts(name);    // display string
    return 0;
}