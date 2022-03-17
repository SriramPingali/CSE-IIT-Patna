; test06.asm
; program first stored var1(25) into B and var2(2) into A (after first two instuctions), then performs A := B << A;
; Final Result 25 << 2 = 100
ldc var1
ldc var2	
shl
HALT
var1: SET 25
var2: SET 2	
