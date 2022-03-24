CS342 - Operating System Lab

Assignment 7
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            24-Mar-2021
======================================

Answer 1:

Filename: prog1.cpp
Compile : g++ prog1.cpp -o prog1
Execute : ./prog1

Input Specification:
    the first line of input will contain a number specifying total processes (n) 
and time quantum. n line follows, ith line contains two space separated integers specifying 
arrival_time and burst_time of ith process.

Sample session:
$ ./prog1
6 2
0 4
1 5
2 2
3 1
4 6
6 3

Avg_WT = 7.33	Avg_TAT = 10.83
P3 P1 P4 P2 P6 P5


$ ./prog1
6 4
0 5
1 6
2 3
3 1
4 5
6 4

Avg_WT = 11.33	Avg_TAT = 15.33
P3 P4 P1 P6 P2 P5 


$ ./prog1
6 3
5 5
4 6
3 7
1 9 
2 2
6 3

Avg_WT = 16.00	Avg_TAT = 21.33
P2 P6 P4 P1 P5 P3 

===========================================================================

Answer 2:

Filename: prog2.cpp
Compile : g++ prog2.cpp -o prog2
Execute : ./prog2

Input Specification:
     the first line of input will contain a number specifying total processes (n). n line 
follows, ith line contains two space separated integers specifying arrival_time, burst_time and priority 
of ith process.

Sample session:
$ ./prog2
4
0 10 5
1 6 4
3 2 2
5 4 0

Avg_WT = 7.75	Avg_TAT = 13.25
P1 P4 P3 P2

$ ./prog2
7
0	3	2
2	5	6
1	4	3
4	2	5
6	9	7
5	4	4
7	10	10

Avg_WT = 7.71	Avg_TAT = 13.00
P1 P3 P6 P4 P2 P5 P7

$ ./prog2
7
0 4 12
1 2 10
2 3 9
3 5 4
4 1 8
5 4 2
6 6 6

Avg_WT = 9.57	Avg_TAT = 13.14
P1 P4 P6 P7 P5 P3 P2 

===========================================================================

Answer 3:

Filename: prog3.cpp
Compile : g++ prog3.cpp -o prog3
Execute : ./prog3

Input Specification:
    the first line of input will contain a number specifying total processes 
(n) and number of queues i.e 2 here, one for system processes and other for 
user processes. n line follows, ith line contains two space separated integers 
specifying arrival time, burst_time and priority queue of ith process.

Sample session:
$ ./prog3
5 2 
0 10 2
3 7 1
4 6 2
12 5 1
18 8 1 

Avg_WT = 10.40	Avg_TAT = 17.60
P2 P4 P5 P3 P1


$ ./prog3
3 2
0 3 2
1 9 2
1 6 1

Avg_WT = 3.33	Avg_TAT = 9.33
P1 P3 P2 

===========================================================================
