0000 00100000         ldc 0x1000
0001 0000000B         a2sp
0002 FFFFFF0A         adj -1
0003 00000000         ldc result
0004 00000003         stl 0
0005 00000000         ldc count
0006 00000004         ldnl 0
0007 0000000D         call main
0008 0000010A         adj 1
0009 00000012         HALT
     ;
000A FFFFFD0A main:   adj -3
000B 00000103         stl 1
000C 00000203         stl 2
000D 00000000         ldc 0           ; zero accumulator
000E 00000003         stl 0
000F FFFFFF0A loop:   adj -1
0010 00000302         ldl 3
0011 00000003         stl 0
0012 00000102         ldl 1
0013 0000000D         call triangle
0014 0000010A         adj 1
0015 00000302         ldl 3
0016 00000005         stnl 0
0017 00000302         ldl 3
0018 00000101         adc 1
0019 00000303         stl 3
001A 00000002         ldl 0
001B 00000101         adc 1
001C 00000003         stl 0
001D 00000002         ldl 0           ; reload it
001E 00000202         ldl 2
001F 00000007         sub
0020 00000010         brlz loop
0021 00000102         ldl 1           ; get return address
0022 0000030A         adj 3
0023 0000000E         return
     ;
0024 FFFFFD0A triangle:adj -3
0025 00000103         stl 1
0026 00000203         stl 2
0027 00000100         ldc 1
0028 00000008         shl
0029 00000302         ldl 3
002A 00000007         sub
002B 00000010         brlz skip
002C 00000302         ldl 3
002D 00000202         ldl 2
002E 00000007         sub
002F 00000203         stl 2
0030 00000202 skip:   ldl 2
0031 0000000F         brz one
0032 00000302         ldl 3
0033 FFFFFF01         adc -1
0034 00000003         stl 0
0035 FFFFFF0A         adj -1
0036 00000102         ldl 1
0037 00000003         stl 0
0038 00000302         ldl 3
0039 FFFFFF01         adc -1
003A 0000000D         call triangle
003B 00000102         ldl 1
003C 00000003         stl 0
003D 00000103         stl 1
003E 00000302         ldl 3
003F 0000000D         call triangle
0040 0000010A         adj 1
0041 00000002         ldl 0
0042 00000006         add
0043 00000102         ldl 1
0044 0000030A         adj 3
0045 0000000E         return
0046 00000100 one:    ldc 1
0047 00000102         ldl 1
0048 0000030A         adj 3
0049 0000000E         return
     ;
004A 0000000A count:  data 10
004B 00000000 result: data 0
