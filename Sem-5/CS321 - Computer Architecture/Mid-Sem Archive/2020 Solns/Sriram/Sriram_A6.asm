.model small
.stack 64
.data
ORG 3000H
tem db 64
.code 
	start:
		MOV DX, 3000H
		IN AL, DX
		CMP AL, 64H
		JNZ START
		INC DX
		OUT DX, AL
	skip:
		HLT
		end start
	.end
