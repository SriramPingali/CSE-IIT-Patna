;/*****************************************************************************
;TITLE: Claims																		;														
;AUTHOR: P. V. Sriram
;ROLL NO.: 1801CS37
;Declaration of Authorship
;This cpp file, test5.cpp, is part of the assignment of CS321 at the 
;department of Computer Science and Engg, IIT Patna . 
; Multiplication of two positive integers
;*****************************************************************************/
loop1: 
	ldc res	; Load res address
   ldnl 0   ; Load res value into A
   adc var1 ; Add var1 value to A
   ldc res  ; Load res address
   stnl 0   ; Store A into res
   ldc var2 ; Load var2 address 
   ldnl 0   ; Load var2 value into A
   adc -1   ; Decrement A by 1
   ldc var2 ; Load var2 address 
   stnl 0   ; Store A into var2
   ldc var2 ; Load var2 address 
   ldnl 0   ; Load var2 value into A
   brz exit ; Branch to exit if A = 0
   br loop1 ; Banch to loop1 otherwise
exit: 
	HALT     ; Terminate 

var1: SET 2
var2: data 3
res: data 0