; SIMPLE Assembly to add numbers
; Different ways are shown
var1: data 10
var2: data 15
ldc var1
ldnl 0
adc 10 ; immediate adding
stl 0 ; storing in memory
adj -1 ; adjusting the SP
memory:
ldl 1 ; retriving stored value
ldc var2 ; getting the second data
ldnl 0
add ; adding numbers taken from memory
HALT
