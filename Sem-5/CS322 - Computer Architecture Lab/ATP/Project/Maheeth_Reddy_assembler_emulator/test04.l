         ; Name: M.Maheeth Reddy
         ; Roll: 1801CS31
         ; This test file was provided for testing purposes.
         ; I am not the author of this file
00000000 00100000         ldc 0x1000
00000001 0000000B         a2sp
00000002 FFFFFF0A         adj -1
00000003 00004B00         ldc result
00000004 00000003         stl 0
00000005 00004A00         ldc count
00000006 00000004         ldnl 0
00000007 0000020D         call main
00000008 0000010A         adj 1
00000009 00000012         HALT
         ;
0000000A FFFFFD0A main:   adj -3
0000000B 00000103         stl 1
0000000C 00000203         stl 2
0000000D 00000000         ldc 0           ; zero accumulator
0000000E 00000003         stl 0
0000000F FFFFFF0A loop:   adj -1
00000010 00000302         ldl 3
00000011 00000003         stl 0
00000012 00000102         ldl 1
00000013 0000100D         call triangle
00000014 0000010A         adj 1
00000015 00000302         ldl 3
00000016 00000005         stnl 0
00000017 00000302         ldl 3
00000018 00000101         adc 1
00000019 00000303         stl 3
0000001A 00000002         ldl 0
0000001B 00000101         adc 1
0000001C 00000003         stl 0
0000001D 00000002         ldl 0           ; reload it
0000001E 00000202         ldl 2
0000001F 00000007         sub
00000020 FFFFEE10         brlz loop
00000021 00000102         ldl 1           ; get return address
00000022 0000030A         adj 3
00000023 0000000E         return
         ;
00000024 FFFFFD0A triangle:adj -3
00000025 00000103         stl 1
00000026 00000203         stl 2
00000027 00000100         ldc 1
00000028 00000008         shl
00000029 00000302         ldl 3
0000002A 00000007         sub
0000002B 00000410         brlz skip
0000002C 00000302         ldl 3
0000002D 00000202         ldl 2
0000002E 00000007         sub
0000002F 00000203         stl 2
00000030 00000202 skip:   ldl 2
00000031 0000140F         brz one
00000032 00000302         ldl 3
00000033 FFFFFF01         adc -1
00000034 00000003         stl 0
00000035 FFFFFF0A         adj -1
00000036 00000102         ldl 1
00000037 00000003         stl 0
00000038 00000302         ldl 3
00000039 FFFFFF01         adc -1
0000003A FFFFE90D         call triangle
0000003B 00000102         ldl 1
0000003C 00000003         stl 0
0000003D 00000103         stl 1
0000003E 00000302         ldl 3
0000003F FFFFE40D         call triangle
00000040 0000010A         adj 1
00000041 00000002         ldl 0
00000042 00000006         add
00000043 00000102         ldl 1
00000044 0000030A         adj 3
00000045 0000000E         return
00000046 00000100 one:    ldc 1
00000047 00000102         ldl 1
00000048 0000030A         adj 3
00000049 0000000E         return
         ;
0000004A 00000A14 count:  data 10
0000004B 00000014 result: data 0
