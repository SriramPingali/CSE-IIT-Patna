// Assignment #1: testme.c
#include <stdio.h>
#include <string.h>

int exploitable( char *arg )
{
	// Make some stack space
	char buffer[10];

	// Now copy the buffer
	strcpy( buffer, arg );

	printf( "The buffer says .. [%s/%p].\n", buffer, &buffer );
	// Return everything fun
	return( 0 );
}

int main( int argc, char **argv )
{
	// Make some stack information
	char a[100], b[100], c[100], d[100];
	// Call the exploitable function

	char str[505];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    fread(str, sizeof(char), 505, badfile);
    
	exploitable(str);

	// Return everything is OK
	return( 0 );
}

