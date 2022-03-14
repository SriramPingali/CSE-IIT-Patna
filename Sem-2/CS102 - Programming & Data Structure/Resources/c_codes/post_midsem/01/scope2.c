#include<stdio.h>
int main() 
{ 
  { 
      int x = 10; 
  } 
  { 
      printf("%d", x);  // Error: x is not accessible here 
  } 
  return 0; 
} 
