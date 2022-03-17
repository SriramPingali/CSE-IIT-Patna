;To sort the nos. in ascending order

.model small
.stack 64
.data
    A DB 0FFH,70H,90H,60H,0FEH,20H,10H,13H,25H,00H
.code
    START :
        MOV AX,@DATA    ;Load Data in temp register
        MOV DS,AX       ;Load data into data into Data Segment
        MOV CX,0009H    ;Initialize counter
    BACK:
        MOV DX,0009H    ;Initialize counter
        LEA SI,A        ;Initialize source index
    BACK1:
        MOV AL,[SI]     ;Load Data
        INC SI          ;Increment Pointer
        CMP AL,[SI]     ;Compare with previous entry
        JC SKIP         ;If carry, skip
        XCHG AL,[SI]    ;Exchange two values
        DEC SI          ;Decrement Pointer
        MOV [SI],AL     ;Store value
        INC SI          ;Increment Pointer
    SKIP:
        DEC DX          ;Decrement pointer
        JNZ BACK1       ;Loop until DX is zero
        LOOP BACK       ;Loop until DX is zero
        INT 03H
    end START
.end