CS342 - Operating System Lab

Assignment 9
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            03-Apr-2021
======================================

Answer 1:

Filename: prog1.c
Compile : gcc prog1.c -o prog1
Execute : ./prog1

Testcase 1:
SIMULATION OF DEADLOCK PREVENTION
Enter number of processes, resources: 3 2
Enter allocation matrix
2 4 5
3 4 5

Enter max matrix
4 3 4
5 6 1

Enter availablity matrix
2 2

-------------
There is no deadlock

--------------------------------------------------------------

Testcase 2:
SIMULATION OF DEADLOCK PREVENTION
Enter number of processes, resources: 4 5
Enter allocation matrix
1 1 0 0
0 1 0 0
1 0 0 0
1 0 1 0
0 0 0 0

Enter max matrix
1 1 0 1
1 1 0 0
1 1 0 1
1 0 1 0
1 1 1 1

Enter availablity matrix
0 0 0 0 1

-------------
- Failing: Mutual exclusion: By allocating required resources to process 0 deadlock is prevented
- Lack of Preemption
- Deadlock can be prevented by allocating needed resources
- Failing: Hold and Wait condition


===========================================================================

Answer 2:

Filename: prog2.c
Compile : gcc prog2.c -o prog2
Execute : ./prog2

Testcase 1:
Enter no. of processes: 4
Enter no. of resources: 5

Enter Total Amount of Resource R1: 2
Enter Total Amount of Resource R2: 1
Enter Total Amount of Resource R3: 1
Enter Total Amount of Resource R4: 2
Enter Total Amount of Resource R5: 1

Enter the request matrix:
P0: 0 1 0 0 1
P1: 0 0 1 0 1 
P2: 0 0 0 0 1
P3: 1 0 1 0 1

Enter the allocation matrix:
P0: 1 0 1 1 0
P1: 1 1 0 0 0
P2: 0 0 0 1 0
P3: 0 0 0 0 0
Deadlock has been detected
Process P0 is deadlocked
--------------------------------------------

Testcase 2:
Enter no. of processes: 5
Enter no. of resources: 3

Enter Total Amount of Resource R1: 10
Enter Total Amount of Resource R2: 5
Enter Total Amount of Resource R3: 7

Enter the request matrix:
P0: 7 4 3
P1: 1 2 2
P2: 6 0 0
P3: 0 1 1
P4: 4 3 1

Enter the allocation matrix:
P0: 0 1 0
P1: 2 0 0
P2: 3 0 2
P3: 2 1 1
P4: 0 0 2
There is no deadlock here
Possible Safe sequence:	P1 -> P3 -> P0 -> P2 -> P4

===========================================================================

Answer 3:

Filename: prog3.c
Compile : gcc prog3.c -o prog3
Execute : ./prog3

Testcase 1:
Enter no. of processes: 5
Enter no. of resources: 3

Enter Total Amount of Resource R1: 10 
Enter Total Amount of Resource R2: 5
Enter Total Amount of Resource R3: 7

Enter the max matrix:
P0: 7 5 3
P1: 3 2 2
P2: 9 0 2
P3: 2 2 2 
P4: 4 3 3

Enter the allocation matrix:
P0: 0 1 0
P1: 2 0 0
P2: 3 0 2 
P3: 2 1 1
P4: 0 0 2

List of possible safe sequences:
P1 -> P3 -> P0 -> P2 -> P4
P1 -> P3 -> P0 -> P4 -> P2
P1 -> P3 -> P2 -> P0 -> P4
P1 -> P3 -> P2 -> P4 -> P0
P1 -> P3 -> P4 -> P0 -> P2
P1 -> P3 -> P4 -> P2 -> P0
P1 -> P4 -> P3 -> P0 -> P2
P1 -> P4 -> P3 -> P2 -> P0
P3 -> P1 -> P0 -> P2 -> P4
P3 -> P1 -> P0 -> P4 -> P2
P3 -> P1 -> P2 -> P0 -> P4
P3 -> P1 -> P2 -> P4 -> P0
P3 -> P1 -> P4 -> P0 -> P2
P3 -> P1 -> P4 -> P2 -> P0
P3 -> P4 -> P1 -> P0 -> P2
P3 -> P4 -> P1 -> P2 -> P0
Total number of safe sequences: 16
---------------------------------------------

Testcase 2:
Enter no. of processes: 5
Enter no. of resources: 4

Enter Total Amount of Resource R1: 9
Enter Total Amount of Resource R2: 13
Enter Total Amount of Resource R3: 10
Enter Total Amount of Resource R4: 11

Enter the max matrix:
P0: 6 0 1 2
P1: 2 7 5 0
P2: 2 3 5 6
P3: 1 6 5 3
P4: 1 6 5 6 

Enter the allocation matrix:
P0: 4 0 0 1
P1: 1 1 0 0 
P2: 1 2 5 4
P3: 0 6 3 3
P4: 0 2 1 2

List of possible safe sequences:
P0 -> P2 -> P3 -> P1 -> P4
P0 -> P2 -> P3 -> P4 -> P1
P0 -> P2 -> P4 -> P1 -> P3
P0 -> P2 -> P4 -> P3 -> P1
Total number of safe sequences: 4

===========================================================================
