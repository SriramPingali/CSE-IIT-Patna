         ; Name: M.Maheeth Reddy
         ; Roll: 1801CS31
         ; I hereby declare that this test file is my own unaided work.
         ; test05.asm
         ; calculate the value of 4*var + 3
00000000 0000010D call calc       ; function call
00000001 00000012 HALT
00000002 00000003 calc:   stl 0           ; store return address (from A) into stack
00000003 00000C00         ldc var         ; load address of variable var
00000004 00000004         ldnl 0          ; load value of var
00000005 00000200         ldc 2
00000006 00000008         shl             ; shift var left twice
00000007 00000301         adc cnst        ; add 3 to it
00000008 00000D00         ldc res         
00000009 00000005         stnl 0          ; store result in res
0000000A 00000002         ldl 0           ; load return address (from stack) into A
0000000B 0000000E         return          ; return to calling address
0000000C 00000214 var: data 2
0000000D 00000014 res: data 0
0000000E 00000313 cnst: SET 3
