;REP, MOVS,CMPS, SCAS, LODS, and STOS

.model small
.stack 64
.data
org 300H
arr db 01H, 02H, 03H, 04H, 05H
.code
	same:
		MOV DX, 01H

	start:
		MOV AX, @DATA 	;Load Data in temp register
		MOV DS, AX 	    ;Load data into data into Data Segment
	movs_:	
		LEA SI, arr    ;Load data into SI 
		MOV DI, 600H
		MOV AX, 00H
		MOV ES, AX
		MOV CX, 05H
		CLD
		REP MOVSB
	cmps_:
		LEA SI, arr    ;Load data into SI 
		MOV DI, 600H
		MOV CX, 05H
		CLD
		REPE CMPSB
	scas_:
		LEA SI, arr    ;Load data into SI 
		MOV DI, 600H
		MOV CX, 05H
		MOV AL, 01H
		CLD
		SCASB
		JNC lods_
		MOV DX, 01H
	lods_:
		INC SI
		LODSB 
	stod_:
		INC DI
		STOSB
		HLT
		end start
	.end
