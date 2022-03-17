/*
 * An application that illustrates calling the factorial function defined elsewhere.
 */

#include <stdio.h>
#include <inttypes.h>
#include <time.h>

long int multiplyNumbers(int n);

int main() {
	
clock_t start, finish;
	
    int num_count, count_start, count_end;
    printf("Enter repeat count (in 10,000s): ");
    scanf("%d", &num_count);
    count_end = num_count * 100000;
    count_start = (num_count - 1) * 100000;

     start=clock();	  
    for (uint64_t i = count_start; i < count_end; i++) {
         multiplyNumbers(i);
    }

   finish=clock();
printf( "Factorial took %f second \n", ((double) (finish-start))/CLOCKS_PER_SEC);	
    return 0;
}


long int multiplyNumbers(int n) {
    if (n>=1)
        return n*multiplyNumbers(n-1);
    else
        return 1;
}

