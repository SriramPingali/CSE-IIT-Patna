.model small
.stack 64
.data 
date1 dw 31D, 01D
date2 dw 28D, 04D
.code
	date:
		MOV AX, @data	;Load Data in temp register
		MOV DS, AX		;Load address into data segment
		LEA SI, date1	;Load first date pointer
		LEA DI, date2	;Load second date pointer
		MOV AX, [SI] 	;Load first date
		MOV BX, [DI] 	;Load second date
		SUB BX, AX 		;BX <- BX - AX
		MOV DX, BX		;DX <- BX 
	month:
		INC SI  		;Increment pointer
		INC SI 	 		;Increment pointer
		MOV AX, [SI] 	;Load first month
		MOV CX, AX 		;CX <- AX
		DEC CX 			;Decrement CX
		JZ next_month 	;If zero, Jump to next_month
		CMP AX, 03D 	;Compare first month to 3
		JC bef_feb_1 	;If carry, Jump to bef_feb_1
		SUB CX, 02 		;CX <- CX - 2
		SUB DX, 59D  	;DX <- DX - 59
	bef_feb_1:
		SUB DX, 31D 	;DX <- DX - 31
		MOV AX, CX 		;AX <- CX
		MOV BL, 02D 	;BL <- 02D
		DIV BL 		    ;AL <- AH / BL remainder
		CMP AH, 00 		;Compare AH with 00
		JNZ next_month 	;If not zero jump to next month
		INC DX 			;Increment DX by 1
		LOOP bef_feb_1 	;Loop until CX = 0
	next_month:
		INC DI  		;Increment pointer
		INC DI    		;Increment pointer
		MOV AX, [DI]    ;Load second month
		MOV CX, AX      ;CX <- AX
		DEC CX          ;Decrement pointer
		JZ exit         ;If zero, jump to exit
		CMP AX, 03D     ;Compare second month to 3
		JC bef_feb_2 	;Jump if carry to bef_feb_2
		SUB CX, 02      ;CX <- CX - 2
		ADD DX, 59D  	;DX <- DX + 59
	bef_feb_2:
		ADD DX, 31D     ;DX <- DX + 31
		MOV AX, CX 	    ;AX <- CX
		MOV BL, 02D 	;BL <- 02
		DIV BL 	        ;AL <- AH / BL remainder
		CMP AH, 00      ;Compare AH with 00
		JNZ exit        ;If not zero jump to next month
		DEC DX          ;Increment DX by 1
		LOOP bef_feb_2  ;Loop until CX = 0
	exit:
		HLT             ;Terminate
		end date
	.end