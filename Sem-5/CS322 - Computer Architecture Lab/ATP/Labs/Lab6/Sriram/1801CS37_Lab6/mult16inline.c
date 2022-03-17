/**************************************************************
 * This procedure uses inline assembly code to multiply two   * 
 * 16-bit integers. It uses the algorithm given in Chapter 1. *
 **************************************************************/
 long mult (int value1, int value2)
 {
     long  product=0;
     asm("repeat1: bsf   %2,%%ecx;  "
         "         jz     done;      "
         "         shll   %%cl,%1;   "
         "         addl   %1,%0;     "
         "         btcl   %%ecx,%2;  "
         "         jmp    repeat1;   "
         "done:                      "
         :"=r"(product)  
         :"r"(value1), "r"(value2), "0"(product)
         :"%ecx","%edx","cc");
     return(product);
 }
