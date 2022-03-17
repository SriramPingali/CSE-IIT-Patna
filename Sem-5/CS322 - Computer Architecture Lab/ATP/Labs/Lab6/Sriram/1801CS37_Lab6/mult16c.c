/*************************************************************
 * This procedure multiplies two 16-bit integers and returns * 
 * their product. It uses the algorithm given in Chapter 1.  *
 *************************************************************/
 long mult(int value1, int value2)
 {
     long  product=0;
     int  i;

     for (i=0; i < 16; i++)
     {
         if (value2 & 1)
             product += value1;
         value1 <<= 1;
         value2 >>= 1;
     }
     return(product);
 }

