#include<stdio.h>
int main() 
{ 
  int x = 1, y = 2, z = 3; 
  printf(" x = %d, y = %d, z = %d \n", x, y, z); 
  { 
       int x = 10; 
       float y = 20; 
       printf(" x = %d, y = %f, z = %d \n", x, y, z); 
       { 
             int z = 100; 
             printf(" x = %d, y = %f, z = %d \n", x, y, z); 
       } 
  } 
  return 0; 
} 
