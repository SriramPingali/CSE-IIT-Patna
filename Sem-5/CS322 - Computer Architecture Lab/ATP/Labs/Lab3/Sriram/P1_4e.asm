;program to convert an 8 bit BCD number into hexadecimal number

.model small
.stack 64
.data
a db 25h
.code
	MOV AX,@DATA
    MOV DS,AX
    LEA SI,A
    LEA DI,A+100H
	MOV BL, [SI];	BL <- [SI]
	AND BL, 0Fh;	BL = BL AND 0F
	MOV AL, [SI];	AL <- [SI]
	AND AL, 0F0h;	BL = AL AND F0
	MOV CL, 04h;	CL = 04
	ROR AL, CL;	Rotate AL
	MOV DL, 0Ah;	DL = 0A
	MUL DL;	AX = AL * DL
	ADD AL, BL;	AL = AL + BL
	MOV [DI], AL;	[DI] <- AL
	HLT;	End of Program
	end
.end