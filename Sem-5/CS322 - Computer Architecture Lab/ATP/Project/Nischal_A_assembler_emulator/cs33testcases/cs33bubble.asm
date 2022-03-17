; SIMPLE Assembly program for bubble sort
call sort ;calling sort
HALT ; program ends
sort:
sp2a  ; a->sp and b->a
stnl 0 ; loading return PC in sp top
adj -1 ; taking sp up by one
outer:
ldc array
a2sp ; sp points to 1st element of array
inner:
ldl 0 ; a has 1st element value
ldl -1 ;b->previous ele, a->next element
sub
brlz next
ldl -1 ; a value restored
ldl 0 ; 
stl -1 ; a (lesser value) comes down
stl 0 ; b values goes to next location
next:
adj -1
ldc number ; get address of number
adc 1 ; add to get last array address
sp2a ; b->last address and a->sp
sub ; b-a
brz skip
br inner
skip:
ldc number
ldnl 0 ; loading the number
adc -1 ; decrementing number
brz complete
ldc number
stnl 0 ; storing back the value
br outer
complete:
ldc number
adc -1
ldnl 0 ; a has return address now
return ; return back to procedure
array:
data 20
data -10
data -15
data 13
data 20
data -0x11
data -25
number: data 7
