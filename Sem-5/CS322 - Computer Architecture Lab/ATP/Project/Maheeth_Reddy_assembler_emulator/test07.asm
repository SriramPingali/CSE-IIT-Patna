; Name: M.Maheeth Reddy
; Roll: 1801CS31
; I hereby declare that this test file is my own unaided work.
; test07.asm
; program to demonstrate error checking
hello:  ldc 0x23     ; 0x23 is hexadecimal value, and is accepted
        stl 0       
        ldc 64
#def:   add         ; invalid label name
        a2sp
        ldnl hello
        swap        ; invalid mnemonic
        adj -1
hello: HALT         ; duplicate label
hello: SET 64       ; duplicate label