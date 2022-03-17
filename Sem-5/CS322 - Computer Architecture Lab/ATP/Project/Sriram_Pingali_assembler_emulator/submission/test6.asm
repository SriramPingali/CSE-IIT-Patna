;/*****************************************************************************
;TITLE: Claims                                                                      ;                                                       
;AUTHOR: P. V. Sriram
;ROLL NO.: 1801CS37
;Declaration of Authorship
;This cpp file, test5.cpp, is part of the assignment of CS321 at the 
;department of Computer Science and Engg, IIT Patna . 
;*****************************************************************************/
call sumN
HALT

sumN:       stl 0       ; store return address (from A) into stack
            adj -1      ; adjust stack pointer
    loop:   ldc num
            ldnl 0      ; load num into A
            ldc sum
            ldnl 0      ; load sum into A, B = num
            add         ; A = B + A
            ldc sum
            stnl 0      ; store new value of sum
            ldc num
            ldnl 0      ; load num into A again
            adc -1      ; decrement A
            brz done    ; if A == 0, stop sum calculation
            ldc num
            stnl 0      ; otherwise update num value
            br loop     ; repeat calculation
    done:   
            adj 1       ; if done, adjust stack pointer
            ldl 0       ; load calling address address to A
            return      ; return to calling address

num: data 100       ; number upto which sum is calculated
sum: data 0         ; sum is stored here