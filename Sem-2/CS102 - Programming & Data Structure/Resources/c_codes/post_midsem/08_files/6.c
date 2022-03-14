// 1  
// SEEK_SET

// Beginning of file

// 2  
// SEEK_CUR

// Current position of the file pointer

// 3  
// SEEK_END

// End of file

#include <stdio.h>

int main () {
   FILE *fp;

   fp = fopen("file.txt","w+");
   fputs("This is tutorialspoint.com", fp);
  
   fseek( fp, 7, SEEK_SET );
   fputs(" C Programming Language", fp);
   fclose(fp);
   
   return(0);
}