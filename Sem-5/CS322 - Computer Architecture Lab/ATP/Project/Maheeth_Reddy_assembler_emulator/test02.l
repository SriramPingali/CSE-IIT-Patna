         ; Name: M.Maheeth Reddy
         ; Roll: 1801CS31
         ; This test file was provided for testing purposes.
         ; I am not the author of this file
         ; test2.asm
         ; Test error handling
         label:
         label: ; duplicate label definiton
00000000          br nonesuch ; no such label
00000001          ldc 08ge ; not a number
00000002          ldc ; missing operand
00000003          add 5 ; unexpected operand
00000004          ldc 5, 6; extra on end of line
         0def: ; bogus label name
         fibble; bogus mnemonic
         0def ; bogus mnemonic
