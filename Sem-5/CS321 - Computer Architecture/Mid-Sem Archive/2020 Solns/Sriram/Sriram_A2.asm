# ORG 2000
	   LXI H,2050
	   MOV A,M	// B<-M
	   CPI 00
	   JNZ LB
	   MVI C,00	// C<-00H
	   INX H
	   MOV A,M	// A<-M

ITER:	   CPI 04
	   JC STORE	// check for carry
	   SUI 04	// A<-A-B
	   INR C	// C<-C+1
	   JMP ITER

STORE:	   STA 3050	// 3050<-A
	   MOV A,C	// A<-C
	   STA 3051	// 3051<-A
	   HLT	// terminate the program

LB:	   CPI 04
	   JC STORE	// check for carry
	   SUI 04	// A<-A-B
	   INR C	// C<-C+1
	   JMP LB
# ORG 2050
# DB 04, 24D
