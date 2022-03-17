         ; Name: M.Maheeth Reddy
         ; Roll: 1801CS31
         ; I hereby declare that this test file is my own unaided work.
         ; test08.asm
         ; program to demonstrate error checking
         LAZER           ; invalid mnemonic
00000001                  ldc 54g ; invalid label name
00000002 00000003         stl 0
00000003                  adj     ; operand required
00000004                  br goob ; undefined label
00000005 00000900         ldc 9
00000006 00000006         add
00000007          good:   HALT 57 ; operand not required
