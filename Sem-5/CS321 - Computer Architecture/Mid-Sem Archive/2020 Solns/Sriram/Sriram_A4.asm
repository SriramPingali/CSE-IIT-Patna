.model small
.stack 64
.data
org 300H
arr dw 0001, 0002, 0003, 0004, 0005
.code
	start:
		MOV AX, @DATA 	;Load Data in temp register
		MOV DS, AX 	    ;Load data into data into Data Segment
		LEA SI, arr    ;Load data into SI 
		MOV AX, 05H	; Immediate Addressing, moving 5 into AX
		MOV BX, AX	; Register Addressing, moving AX content into BX
		MOV AX, ds:[0300H];Direct Addressing, moving [BX] content into AX
		MOV BX, 0300H; Immediate Addressing, moving 300 into BX
		MOV AX, ds:[BX];Indirect Addressing, moving [BX] content into AX
		MOV AX, ds:[BX + 04H];Base Addressing, moving [BX + 04] content into AX
		MOV AX, ds:[SI + 04H];Index Addressing, moving[SI + 04] content into AX
		MOV AX, ds:[BX + SI + 01H];Base Index addressing
		end start
	.end
