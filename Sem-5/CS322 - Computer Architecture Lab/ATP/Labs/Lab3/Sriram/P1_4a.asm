;Addition of two 16-bit nos

.model small
.stack 64
.data
    A DW 9384H
    B DW 1845H
    SUM DW ?
    CARRY DB 00H
.code
START:    
    MOV AX, @data   ;Load Data in temp register
    MOV DS, AX      ;Load data into data into Data Segment
    MOV AX, A       ;Move from A into accumulator
    ADD AX, B       ;Add eith B
    JNC SKIP        ;If no carry, skip
    INC CARRY       ;If carry present
SKIP:   
    MOV SUM, AX     ;Store the answer
    INT 03H
    end start       ;Terminate
.end