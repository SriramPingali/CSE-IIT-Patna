;program to find the factorial of a number

.model small
.stack 64
.data
a dw 0004h
.code
	START:
		MOV AX,@DATA
	    MOV DS,AX
	    LEA SI,A
	    LEA DI,A+100H
	    MOV CX, [SI];	CX <- [0500]
		MOV AX, 0001;	AX <- 0001
		MOV DX, 0000;	DX <- 0000
	L1:
		MUL CX;	DX:AX <- AX * CX
		DEC CX
		JNZ L1	
		MOV [DI], AX;	[0600]<-AX
		MOV [DI + 1], DX;	[0601]<-DX
		HLT;	Stop Execution
	end start
.end