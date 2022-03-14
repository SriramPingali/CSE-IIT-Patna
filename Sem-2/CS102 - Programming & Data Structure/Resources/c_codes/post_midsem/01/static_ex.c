#include<stdio.h> 
int fun() 
{ 
  static int count = 0; 
  count++; 
  return count; 
} 
   
int main() 
{ 
  printf("%d ", fun()); 
  printf("%d ", fun()); 
  return 0; 
}

// without static 


// #include<stdio.h> 
// int fun() 
// { 
//   int count = 0; 
//   count++; 
//   return count; 
// } 
   
// int main() 
// { 
//   printf("%d ", fun()); 
//   printf("%d ", fun()); 
//   return 0; 
// }
