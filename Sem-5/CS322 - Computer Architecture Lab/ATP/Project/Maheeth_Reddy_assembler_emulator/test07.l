         ; Name: M.Maheeth Reddy
         ; Roll: 1801CS31
         ; I hereby declare that this test file is my own unaided work.
         ; test07.asm
         ; program to demonstrate error checking
00000000 00002300 hello:  ldc 0x23     ; 0x23 is hexadecimal value, and is accepted
00000001 00000003         stl 0       
00000002 00004000         ldc 64
00000003 00000006 #def:   add         ; invalid label name
00000004 0000000B         a2sp
00000005 00000004         ldnl hello
                 swap        ; invalid mnemonic
00000007 FFFFFF0A         adj -1
00000008 00000012 hello: HALT         ; duplicate label
00000009          hello: SET 64       ; duplicate label
