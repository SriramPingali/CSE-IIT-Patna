; test07.asm
; Simple arithmetic Program
ldc var1	;store 100 in A
adc 20		;add 20 to A

ldc var2	;load 2 in A, shift A value to B
adc	-1		;sub 1 from A

;Now A = 1 and B = 120 (both in decimal)
shr

;A will containd 60 (as 120 >> 1 = 60)
HALT

var1: SET 100
var2: SET 2	
