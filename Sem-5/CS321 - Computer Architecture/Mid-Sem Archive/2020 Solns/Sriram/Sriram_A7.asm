.model small
.stack 64
.data
	start db "Experimenting with Interrupyts!" , "$"
.code
	MOV AX, @data	;Load Data in temp register
	MOV DS, AX		;Load address into data segment
	LEA SI, start	;Load first date pointer
	lea dx , [SI]
	mov ah , 09h
	int 21h
	HLT
	end
.end