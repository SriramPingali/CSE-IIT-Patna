     ; test1.asm
0000          label:	; an unused label
0000 00000000 	ldc 0
0001 FFFFFB00 	ldc -5
0002 00000500 	ldc +5
0003 00000011 loop: br loop ; an infinite loop
0004 00000011 br next	;offset should be zero
0005          next:
0005 00000000     	ldc loop ; load code address
0006 00000000 	ldc var1 ; forward ref
0007 00000000 var1: data 0 ; a variable
