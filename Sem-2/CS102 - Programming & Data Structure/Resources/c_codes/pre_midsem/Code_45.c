//https://www.geeksforgeeks.org/wp-content/uploads/Function-Prototype-in-c.png
#include<stdio.h>
// function prototype, also called function declaration
float square ( float x );                               
// main function, program starts from here
 
int main( )               
{
    float m, n ;
    printf ( "\nEnter some number for finding square \n");
    scanf ( "%f", &m ) ;
    // function call
    n = square ( m ) ;                      
    printf ( "\nSquare of the given number %f is %f",m,n );
}
 
float square ( float x )   // function definition
{
    float p ;
    p = x * x ;
    return ( p ) ;
}

/* 
#include<stdio.h>

int multiply(int a, int b);     // function declaration

int main() 
{
    int i, j, result;
    printf("Please enter 2 numbers you want to multiply...");
    scanf("%d%d", &i, &j);
    
    result = multiply(i, j);        // function call
    printf("The result of muliplication is: %d", result);
    
    return 0;
}

int multiply(int a, int b)
{
    return (a*b);       // function defintion, this can be done in one line
} */