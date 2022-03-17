/*
 * An application that illustrates calling the factorial function defined elsewhere.
 */

#include <stdio.h>
#include <inttypes.h>


uint64_t factorial(uint64_t n);

int main() {
	
     start=clock();	  
    for (uint64_t i = 0; i < 20; i++) {
          factorial(i);
    }

   finish=clock();
Prntf( "Factorial took %f second \n", ((double) (finish-start))/CLOCKS_PER_SEC);	
    return 0;
}
