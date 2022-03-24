CS342 - Operating System Lab

Assignment 10
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            06-Apr-2021
======================================

Note: Name of the input file must be "pages.txt" for the program to read inputs properly.
      It would be better if input file doesn't end with a newline.

Answer 1:

Filename: Q1_1.cpp
Compile : g++ Q1_1.cpp -o Q1_1
Execute : ./Q1_1

Output:
FIFO:

Testcase 1:
Capacity: 3
4 7 6 1 7 6 1 2 7 2 

Frame content at each time step t
F1	F2	F3	
X	X	X	at t = 0
4	X	X	at t = 1
4	7	X	at t = 2
4	7	6	at t = 3
1	7	6	at t = 4
1	7	6	at t = 5
1	7	6	at t = 6
1	7	6	at t = 7
1	2	6	at t = 8
1	2	7	at t = 9
1	2	7	at t = 10

#Page Faults: 6
---------------

Testcase 2:
Capacity: 4
1 2 3 4 2 7 5 1 1 6 4 7 2 1 2 5 

Frame content at each time step t
F1	F2	F3	F4	
X	X	X	X	at t = 0
1	X	X	X	at t = 1
1	2	X	X	at t = 2
1	2	3	X	at t = 3
1	2	3	4	at t = 4
1	2	3	4	at t = 5
7	2	3	4	at t = 6
7	5	3	4	at t = 7
7	5	1	4	at t = 8
7	5	1	4	at t = 9
7	5	1	6	at t = 10
4	5	1	6	at t = 11
4	7	1	6	at t = 12
4	7	2	6	at t = 13
4	7	2	1	at t = 14
4	7	2	1	at t = 15
5	7	2	1	at t = 16

#Page Faults: 13
---------------

===========================================================================

Answer 2:

Filename: Q1_2.cpp
Compile : g++ Q1_2.cpp -o Q1_2
Execute : ./Q1_2

Output:
LRU:

Testcase 1:
Capacity: 3
4 7 6 1 7 6 1 2 7 2 

Frame content at each time step t
F1	F2	F3	
X	X	X	at t = 0
4	X	X	at t = 1
4	7	X	at t = 2
4	7	6	at t = 3
1	7	6	at t = 4
1	7	6	at t = 5
1	7	6	at t = 6
1	7	6	at t = 7
1	2	6	at t = 8
1	2	7	at t = 9
1	2	7	at t = 10

#Page Faults: 6
---------------

Testcase 2:
Capacity: 4
1 2 3 4 2 7 5 1 1 6 4 7 2 1 2 5 

Frame content at each time step t
F1	F2	F3	F4	
X	X	X	X	at t = 0
1	X	X	X	at t = 1
1	2	X	X	at t = 2
1	2	3	X	at t = 3
1	2	3	4	at t = 4
1	2	3	4	at t = 5
7	2	3	4	at t = 6
7	2	5	4	at t = 7
7	2	5	1	at t = 8
7	2	5	1	at t = 9
7	6	5	1	at t = 10
4	6	5	1	at t = 11
4	6	7	1	at t = 12
4	6	7	2	at t = 13
4	1	7	2	at t = 14
4	1	7	2	at t = 15
5	1	7	2	at t = 16

#Page Faults: 13
---------------

===========================================================================

Answer 3:

Filename: Q1_3.cpp
Compile : g++ Q1_3.cpp -o Q1_3
Execute : ./Q1_3

Output:
Optimal:

Testcase 1:
Capacity: 3
4 7 6 1 7 6 1 2 7 2 

Frame content at each time step t
F1	F2	F3	
X	X	X	at t = 0
4	X	X	at t = 1
4	7	X	at t = 2
4	7	6	at t = 3
1	7	6	at t = 4
1	7	6	at t = 5
1	7	6	at t = 6
1	7	6	at t = 7
2	7	6	at t = 8
2	7	6	at t = 9
2	7	6	at t = 10

#Page Faults: 5
---------------

Testcase 2:
Capacity: 4
1 2 3 4 2 7 5 1 1 6 4 7 2 1 2 5 

Frame content at each time step t
F1	F2	F3	F4	
X	X	X	X	at t = 0
1	X	X	X	at t = 1
1	2	X	X	at t = 2
1	2	3	X	at t = 3
1	2	3	4	at t = 4
1	2	3	4	at t = 5
1	2	7	4	at t = 6
1	5	7	4	at t = 7
1	5	7	4	at t = 8
1	5	7	4	at t = 9
1	6	7	4	at t = 10
1	6	7	4	at t = 11
1	6	7	4	at t = 12
1	2	7	4	at t = 13
1	2	7	4	at t = 14
1	2	7	4	at t = 15
5	2	7	4	at t = 16

#Page Faults: 9
---------------

===========================================================================

Report on performance statistics:
For testcase 1:
3
4 7 6 1 7 6 1 2 7 2

Total References = 10

Scheme    Page faults     Hit rates
------------------------------------
FIFO          6              40%
LRU           6              40%
Optimal       5              50%

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
For testcase 2:
4
1 2 3 4 2 7 5 1 1 6 4 7 2 1 2 5

Total References = 16

Scheme    Page faults     Hit rates
------------------------------------
FIFO          13           18.75%
LRU           13           18.75%
Optimal        9           43.75%

