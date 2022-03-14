#include<stdio.h>

void main()
{
  int a=5;

  printf("Address is %p:",&a);
  printf("\nValue at the address %p is %d:",&a,*&a);
  
}