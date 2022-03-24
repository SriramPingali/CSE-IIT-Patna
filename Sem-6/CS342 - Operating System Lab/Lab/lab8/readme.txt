CS342 - Operating System Lab

Assignment 8
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            31-Mar-2021
======================================

Answer 1:

Filename: prog1.c
Compile : gcc prog1.c -o prog1 -pthread
Execute : ./prog1

Sample session:
$ ./prog1
Lock acquired by pthread1
pthread1 is accessing function f()
pthread2 cannot access function f() lock acquired by pthread1
Lock released by pthread1
Lock acquired by pthread2
pthread2 is accessing function f()
Lock released by pthread2

===========================================================================

Answer 2:

Filename: prog2.c
Compile : gcc prog2.c -o prog2 -pthread
Execute : ./prog2

Sample session:
$ ./prog2
Sum of primes in thread 0: 420812
Sum of primes in thread 1: 1127324
Sum of primes in thread 2: 1749901
Sum of primes in thread 3: 2438359
Sum of all primes from 0 to 10000 is 5736396

===========================================================================

Answer 3:

Filename: prog3.c
Compile : gcc prog3.c -o prog3 -pthread
Execute : ./prog3

Sample session:
$ ./prog3
Philosopher 1 starts thinking
Philosopher 2 starts thinking
Philosopher 3 starts thinking
Philosopher 4 starts thinking
Philosopher 5 starts thinking
Philosopher 2 is hungry
Request Matrix:
0 0 0 0 0 
1 1 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 4 is hungry
Request Matrix:
0 0 0 0 0 
1 1 0 0 0 
0 0 0 0 0 
0 0 1 1 0 
0 0 0 0 0 

Philosopher 1 is hungry
Request Matrix:
1 0 0 0 1 
1 1 0 0 0 
0 0 0 0 0 
0 0 1 1 0 
0 0 0 0 0 

Philosopher 5 is hungry
Request Matrix:
1 0 0 0 1 
1 1 0 0 0 
0 0 0 0 0 
0 0 1 1 0 
0 0 0 1 1 

Philosopher 3 is hungry
Request Matrix:
1 0 0 0 1 
1 1 0 0 0 
0 1 1 0 0 
0 0 1 1 0 
0 0 0 1 1 

A deadlock has been detected and recovery begins

Philosopher 3 ends eating and releases forks 2 (left) and 3 (right) down
Allocation Matrix:
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 3 starts thinking
Request Matrix:
1 0 0 0 1 
1 1 0 0 0 
0 0 0 0 0 
0 0 1 1 0 
0 0 0 1 1 

Philosopher 4 grabs fork 3 and 4
Allocation Matrix:
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 1 1 0 
0 0 0 0 0 

Philosopher 4 starts eating
Philosopher 4 ends eating and releases forks 3 (left) and 4 (right) down
Allocation Matrix:
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 4 starts thinking
Request Matrix:
1 0 0 0 1 
1 1 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 1 1 

Philosopher 5 grabs fork 4 and 5
Allocation Matrix:
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 1 1 

Philosopher 5 starts eating
Philosopher 5 ends eating and releases forks 4 (left) and 5 (right) down
Allocation Matrix:
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 5 starts thinking
Request Matrix:
1 0 0 0 1 
1 1 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 1 grabs fork 5 and 1
Allocation Matrix:
1 0 0 0 1 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 1 starts eating
Philosopher 1 ends eating and releases forks 5 (left) and 1 (right) down
Allocation Matrix:
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 1 starts thinking
Request Matrix:
0 0 0 0 0 
1 1 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 2 grabs fork 1 and 2
Allocation Matrix:
0 0 0 0 0 
1 1 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 2 starts eating
Philosopher 2 ends eating and releases forks 1 (left) and 2 (right) down
Allocation Matrix:
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

Philosopher 2 starts thinking
Request Matrix:
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 
0 0 0 0 0 

===========================================================================
