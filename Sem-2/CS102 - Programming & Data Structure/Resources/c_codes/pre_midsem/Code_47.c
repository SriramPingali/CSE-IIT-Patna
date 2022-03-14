
#include <stdio.h>
/* function declaration */
void swap(int x, int y);
void main ()
{
/* local variable definition */
int a = 100;
int b = 200;
printf("Before swap, value of a : %d\n", a );
printf("Before swap, value of b : %d\n", b );
/* calling a function to swap the values */
swap(a, b);
printf("After swap, value of a : %d\n", a );
printf("After swap, value of b : %d\n", b ); 
}

/* function definition to swap the values */
void swap(int x, int y)
{
int temp;
temp = x; /* save the value of x */
x = y; /* put y into x */
y = temp; /* put x into y */

printf("In swap function, value of x : %d\n", x );
printf("In swap function, value of y : %d\n",y ); 

}
