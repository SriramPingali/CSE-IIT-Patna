; SIMPLE asm for showing branch
var1: data -10
ldc var1
ldnl 0 ; a will have -10
ldc 10 ; storing 10 in a and -10 in b
add
brz next
temp:
adc 15 ; adding immediate if not zero
adc -40 ; adding anothter immediate
brlz end
next:
adc 20
br temp
end:
HALT

