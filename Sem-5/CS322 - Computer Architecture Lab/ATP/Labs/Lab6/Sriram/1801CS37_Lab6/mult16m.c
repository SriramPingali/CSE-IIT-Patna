/*****************************************************
 * This program calls the multiply procedure a large *
 * number of times and prints the execution time.    *
 *****************************************************/
#include        <stdio.h>
#include        <time.h>

int main(void)
{
        clock_t start, finish;
        int value1=1000, value2=4096;
        int i, j, n;

        extern long mult(int, int);

        printf ("Please input repeat count: ");
        scanf("%d", &n);

        start = clock();   /* start clock */
        for (j=0; j<n; j++)
            for (i=0; i<1000000; i++)
                mult(value1, value2);
        finish = clock();  /* stop clock */

        printf("Multiplication took %f seconds to finish.\n", 
        	((double)(finish-start))/ CLOCKS_PER_SEC);
	
        return 0;
}

