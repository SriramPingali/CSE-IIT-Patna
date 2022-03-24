CS342 - Operating System Lab

Assignment 5
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            16-Feb-2021
======================================

Answer 1:

Filename: ques1.c
Compile: gcc -o ques1 ques1.c -lm -pthread -fopenmp
Execute: ./ques1 [N] 
                 (N is number of threads, specified by user)

Sample session:
$ ./ques1 12
I am groot 1536460608. Created new thread (1527977728) in iteration 0...
Hello from groot 1527977728 - I was created in iteration 0
I am groot 1536460608. Created new thread (1517389568) in iteration 1...
I am groot 1536460608. Created new thread (1508996864) in iteration 2...
Hello from groot 1517389568 - I was created in iteration 1
Hello from groot 1508996864 - I was created in iteration 2
I am groot 1536460608. Created new thread (1500604160) in iteration 3...
I am groot 1536460608. Created new thread (1492211456) in iteration 4...
Hello from groot 1500604160 - I was created in iteration 3
Hello from groot 1492211456 - I was created in iteration 4
I am groot 1536460608. Created new thread (1409283840) in iteration 5...
Hello from groot 1409283840 - I was created in iteration 5
I am groot 1536460608. Created new thread (1400891136) in iteration 6...
Hello from groot 1400891136 - I was created in iteration 6
I am groot 1536460608. Created new thread (1392498432) in iteration 7...
Hello from groot 1392498432 - I was created in iteration 7
I am groot 1536460608. Created new thread (1384105728) in iteration 8...
Hello from groot 1384105728 - I was created in iteration 8
I am groot 1536460608. Created new thread (1375713024) in iteration 9...
Hello from groot 1375713024 - I was created in iteration 9
I am groot 1536460608. Created new thread (1367320320) in iteration 10...
Hello from groot 1367320320 - I was created in iteration 10
I am groot 1536460608. Created new thread (1358927616) in iteration 11...
Hello from groot 1358927616 - I was created in iteration 11

===========================================================================

Answer 2:

Filename: ques2.c
Compile: gcc -o ques2 ques2.c -lm -pthread -fopenmp
Execute: ./ques2

Sample Session:
$ ./ques2
Final Result:	global_var=10	local_var=10

===========================================================================

Answer 3:

Filename: ques3.c
Compile: gcc -o ques3 ques3.c -lm -pthread -fopenm
Execute: ./ques3

Sample Session:
$ ./ques3
Computation under process. Please wait...
cbrt(0) = 0.000000
cbrt(1) = 1.000000
cbrt(2) = 1.259921
cbrt(3) = 1.442250
cbrt(4) = 1.587401
cbrt(5) = 1.709976
cbrt(6) = 1.817121
cbrt(7) = 1.912931
cbrt(8) = 2.000000
cbrt(9) = 2.080084
cbrt(10) = 2.154435
cbrt(11) = 2.223980
cbrt(12) = 2.289428
cbrt(13) = 2.351335
cbrt(14) = 2.410142
cbrt(15) = 2.466212
cbrt(16) = 2.519842
..........
..........
..........
cbrt(990) = 9.966555
cbrt(991) = 9.969910
cbrt(992) = 9.973262
cbrt(993) = 9.976612
cbrt(994) = 9.979960
cbrt(995) = 9.983305
cbrt(996) = 9.986649
cbrt(997) = 9.989990
cbrt(998) = 9.993329
cbrt(999) = 9.996666
===========================================================================