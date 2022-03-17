; important
; demonstrates the use of SET
br start
data 0x5ab4
start:
ldc 0x65
adc 0x9d
val: SET 75
ldc val
adc val2
val2: SET 66
