.model small
.stack 64
.data
arr1 db 05h,03h,07h,01h,09h,02h,06h,08h,04h,0Ah
.code
	start:	
		MOV AX, @DATA 	;Load Data in temp register
		MOV DS, AX 	    ;Load data into data into Data Segment
		LEA SI, arr1    ;Load data into SI 
		MOV CX, 10 		;Initialize counter
		MOV DL, 10  	;DL <- 10
		MOV AH, 00h		;AX = 0000
	L1:	
		ADD AH, [SI] 	;Add data into accumulator
		INC SI  		;Increment pointer
		DEC CX 			;Decrement counter
		JNZ L1   		;Loop until CX == 0
		MOV BH, AH      ;Store the answer	
		MOV AH,4CH
      	INT 21H
		end start
	.end