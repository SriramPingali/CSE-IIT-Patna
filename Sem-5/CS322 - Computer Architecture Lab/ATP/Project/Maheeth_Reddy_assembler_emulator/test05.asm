; Name: M.Maheeth Reddy
; Roll: 1801CS31
; I hereby declare that this test file is my own unaided work.
; test05.asm
; calculate the value of 4*var + 3
call calc       ; function call
HALT

calc:   stl 0           ; store return address (from A) into stack
        ldc var         ; load address of variable var
        ldnl 0          ; load value of var
        ldc 2
        shl             ; shift var left twice
        adc cnst        ; add 3 to it
        ldc res         
        stnl 0          ; store result in res
        ldl 0           ; load return address (from stack) into A
        return          ; return to calling address

var: data 2
res: data 0
cnst: SET 3