; Name: M.Maheeth Reddy
; Roll: 1801CS31
; This test file was provided for testing purposes.
; I am not the author of this file
; test2.asm
; Test error handling
label:
label: ; duplicate label definiton
br nonesuch ; no such label
ldc 08ge ; not a number
ldc ; missing operand
add 5 ; unexpected operand
ldc 5, 6; extra on end of line
0def: ; bogus label name
fibble; bogus mnemonic
0def ; bogus mnemonic
