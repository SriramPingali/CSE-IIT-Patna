// Assignment #1: mangesh.c
#include <stdio.h>
#include <time.h>
#include <assert.h>

int main( int argc, char **argv )
{
	printf("\n*****************************************\n");
	
	printf("\nMangesh Chandrawanshi");
	printf("\nCS547-Foundation of Computer Security\n");
	
	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    assert(strftime(s, sizeof(s), "%c", tm));
    printf("%s\n", s);

	printf("\n*****************************************\n");

	// Return everything is OK
	return( 0 );
}

