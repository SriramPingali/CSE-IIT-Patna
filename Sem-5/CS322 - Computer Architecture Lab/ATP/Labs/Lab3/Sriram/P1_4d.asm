; To check for a Palindrome (single word, 5 letters)

.MODEL SMALL
.STACK 64
.DATA
    A DB 'M','A','D','A','M'    
.CODE
    START:    
        MOV AX,@DATA    ;Load Data in temp register
        MOV DS,AX       ;Load data into data into Data Segment
        MOV CH,00H      ;Answer pointer
        LEA SI,A        ;Load data into SI
        LEA DI,A+04H    ;Load data into DI
        MOV CL,02H      ;CL <- 02H
    BACK:    
        MOV AH,[SI]     ;Load data into AH
        MOV BH,[DI]     ;Load data into BH
        CMP AH,BH       ;Compare AH, BH
        JNZ SKIP        ;If not zero skip
        INC SI          ;Increment pointer
        DEC DI          ;Decrement pointer
        DEC CL          ;Decrement pointer
        JNZ BACK        ;If not zero, jump to back 
        INC CH          ;Increment of answer
    SKIP:    
        INT 03H
        END START
.END