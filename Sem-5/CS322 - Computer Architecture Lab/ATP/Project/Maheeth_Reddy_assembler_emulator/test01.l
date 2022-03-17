         ; Name: M.Maheeth Reddy
         ; Roll: 1801CS31
         ; This test file was provided for testing purposes.
         ; I am not the author of this file
         ; test1.asm
         label:	; an unused label
00000000 00000000 	ldc 0
00000001 FFFFFB00 	ldc -5
00000002 00000500 	ldc +5
00000003 FFFFFF11 loop: br loop ; an infinite loop
00000004 00000011 br next	;offset should be zero
         next:
00000005 00000300     	ldc loop ; load code address
00000006 00000700 	ldc var1 ; forward ref
00000007 00000014 var1: data 0 ; a variable
