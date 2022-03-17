; 8085 Program to sort numbers in ascending order using Selection Sort

; clear registers
MVI A,00H
LXI B,0000H
LXI D,0000H

; count of numbers in input
; is stored at location 2000
; load address 2000 in HL pair
LXI H,2000H

; Store value at address 2000 in C
MOV C,M
CMP C
; if C is 0, quit program
; because no numbers to sort
JZ QUIT

; address of stack pointer is set to 4000h
LXI SP,4000H

; Loop till the end of given numbers,
; starting from i-th number (0 <= i <= C-1).
; Find the minimum and swap
; with i-th number if lesser
LOOP:
    ; increment HL pair to next location
    INX H
    ; copy value in M to A
    MOV A,M
    CALL FIND_MIN_NUM
    ; compare min number with A
    CMP M
    ; if both are equal dont swap
    JZ DONT_SWAP
    ; otherwise swap
    CALL SWAP
DONT_SWAP:
    DCR C
    JNZ LOOP

QUIT:    HLT

; function to find the minimum
; number in the array by iterating
; through each number until the end
FIND_MIN_NUM:
    ; store current values of
    ; H and B on stack
    ; since we use H,B in this function
    PUSH H
    PUSH B

    ; C stores count of remaining numbers
    DCR C
ITER:
    ; increment HL pair
    INX H
    ; compare A and M
    CMP M
    ; if equal don't update A
    JC SKIP
    ; update A
    MOV A,M
    ; store location of this new minimum in DE
    MOV D,H
    MOV E,L
SKIP:
    DCR C
    JNZ ITER

    ; restore H,B
    POP B
    POP H
    RET

; function to swap 2 numbers
; in memory
SWAP:
    ; push program status to stack
    PUSH PSW
    ; push value in BC to stack
    PUSH B
    ; load data into A from
    ; location pointed by DE
    LDAX D
    ; copy value in A to B
    MOV B,A
    ; copy value at location HL to A
    MOV A,M
    ; store value in A at location DE
    STAX D
    ; store value in B at location HL
    MOV M,B
    ; pop top of stack to BC
    POP B
    ; program status is restored
    ; in calling function
    POP PSW
    RET