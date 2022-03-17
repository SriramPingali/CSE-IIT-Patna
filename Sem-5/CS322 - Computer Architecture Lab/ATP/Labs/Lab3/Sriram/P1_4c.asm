;To find the no of even & odd nos. from series of 10 nos

.model small
.stack 64
.data
    A DB 10H,15H,25H,16H,17H,19H,23H,77H,47H,34H
.code
    START:    
        MOV AX,@DATA    ;Load Data in temp register
        MOV DS,AX       ;Load data into data into Data Segment
        LEA SI,A        ;Load valus into SI
        MOV BX,0000H    ;Initialize BX with 0
        MOV CX,000AH    ;Initialize pointer
    BACK:    
        MOV AL,[SI]     ;Load data into Accumulator
        ROR AL,1        ;Rotate right
        JC ODD          ;If carry, Jump to ODD
        INC BL          ;Increment BL
        JMP NEXT        ;Jump to next
    ODD:    
        INC BH          ;Increment BH
    NEXT:
        INC SI          ;Increment SI
        LOOP BACK       ;Loop until pointer is 0
        INT 03H         
        end START
.end