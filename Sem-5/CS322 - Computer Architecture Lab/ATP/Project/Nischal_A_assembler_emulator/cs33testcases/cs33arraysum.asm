; for finding sum of an array
var1:
data 5 ; number of elements 
data 5
data 15
data 10
data 20
data 10
result:
data 0 ; data to store the result
main:
ldc 0 ;making a and b zero
adj 1
loop:
ldc result ;loading result in a
ldnl 0 ; a has result
ldl 0 ; b has result and new num loaded
add ; adding b with a
ldc result
stnl 0
ldc var1
adj 1
sp2a
sub
brz finish
br loop
finish:
ldc result
a2sp
HALT
