/* stack.c */
/* This program has a buffer overflow vulnerability. */
/* Our task is to exploit this vulnerability */
// gcc -g -o stack --fno-stack-protector stack.c (For no stack guard)
// gcc -z execstack -o stack stack.c (For executable stack)
// gcc -g -o stack -z execstack -fno-stack-protector stack.c (For both)
// chmod 4755 stack
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int bof(char *str)
{
	char buffer[24];
	/* The following statement has a buffer overflow problem */
	strcpy(buffer, str);
	return 1;
}	

int main(int argc, char **argv)
{
	char str[517];
	FILE *badfile;
	badfile = fopen("badfile", "r");
	fread(str, sizeof(char), 517, badfile);
	bof(str);
	printf("Returned Properly\n");
	return 1;
}