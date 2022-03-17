; Name: M.Maheeth Reddy
; Roll: 1801CS31
; I hereby declare that this test file is my own unaided work.
; test08.asm
; program to demonstrate error checking
LAZER           ; invalid mnemonic
        ldc 54g ; invalid label name
        stl 0
        adj     ; operand required
        br goob ; undefined label
        ldc 9
        add
good:   HALT 57 ; operand not required