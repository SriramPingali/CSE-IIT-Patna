;assembly language program to  find sum of 10 random  numbers.

.model small
.stack 64
.data
arr1 dw 0001, 0002, 0003, 0004, 0005
.code
	start:	
		MOV AX, @DATA 	;Load Data in temp register
		MOV DS, AX 	    ;Load data into data into Data Segment
		LEA SI, arr1    ;Load data into SI 
		MOV CX, 05H 	;CX <-05H
		MOV DL, 05H     ;DL <-05H
		MOV AX, 0000 	;AX = 0000
	L1:	
		ADD AX, [SI] 	;Add data to accumulator
		INC SI          ;Increment pointer
		INC SI          ;Increment pointer
		DEC CX          ;Decrement pointer
		JNZ L1          ;If not zero, Jump to L1
		DIV DL          ;Divide Acc with DL
		MOV BX, AX      ;Store value
		MOV AH,4CH
      	INT 21H
		end start
	.end