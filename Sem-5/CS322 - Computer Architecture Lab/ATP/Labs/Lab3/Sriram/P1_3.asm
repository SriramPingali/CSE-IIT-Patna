;assembly language program to  find  average  of  the  given set of 16 bit  ;number  numbers.

.model small
.stack 64
.data
arr1 db 5,3,7,1,9,2,6,8,4,10
.code
	start:	
		MOV AX, @DATA 	;Load Data in temp register
		MOV DS, AX 	    ;Load data into data into Data Segment
		LEA SI, arr1    ;Load data into SI 
		MOV CL, 10      ;Initialize counter
		MOV AL, [SI]    ;Load data into AL
		MOV BL, [SI]    ;Load data into BL
    update_min:
    	CMP AL, [SI]    ;Compare data to AL
    	JC update_max   ;If carry, Jump to Update Max
		MOV AL, [SI]	;Store data into AL
	update_max:
		CMP BL, [SI]    ;Compare data to BL
		JNC L1          ;If carry, Jump to L1
		MOV BL, [SI]    ;Store data into BL
	L1:	
		INC SI  		;Increment pointer
		DEC CX          ;Decrement counter
		JNZ update_min  ;If not zero, jump to update_min
		SUB BL, AL      ;BL <- BL - AL
		MOV DL, BL 		;Store answer into DL
		MOV AH,09H
		INT 21H
		end start
	.end