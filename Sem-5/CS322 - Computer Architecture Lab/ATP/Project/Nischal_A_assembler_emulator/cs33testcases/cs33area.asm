; calculating area of rect and sq
; Demostrates CALL, RETURN and LOOP operations
length: data 20
bredth: data 10
area: data 0
call rectangle ;calling the function
HALT
rectangle:
sp2a  ; a->sp and b->a
stnl 0 ; loading return PC in sp top
adj -1 ; taking sp up by one
loop:
ldc length
ldnl 0
ldc area
ldnl 0 ; b -> length, a-> area
add
ldc area
stnl 0 ; loading back the area
ldc bredth
ldnl 0
adc -1 ; decrementing bredth
brz complete
ldc bredth
stnl 0 ; storing back the value
br loop
complete:
adj 1 ; pointing to return PC address
sp2a
ldnl 0 ; a has return address
return ; return back to procedure
