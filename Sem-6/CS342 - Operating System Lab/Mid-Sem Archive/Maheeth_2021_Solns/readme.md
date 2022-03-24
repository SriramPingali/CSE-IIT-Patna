#	CS342 - Operating System Lab
#        Midsem Assignment
       -------------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            16-Mar-2021
======================================

Answer 1:

Filename: midsem-q1.c
Compile : gcc midsem-q1.c -o midsem-q1
Execute : ./midsem-q1

Sample session:
$ ./midsem-q1
Parent process is forked
This is Child Process 1: PID=2925	Parent PID=2924
Child process 1 is quitting
This is the Parent Process: PID=2924
Parent Process is a ZOMBIE process now

Parent process is forked once again
Parent Process (PID=2924) is quitting
This is Child Process 2: PID=2926	Parent PID=2924
Child Process 2 is an ORPHAN process now

===========================================================================

Answer 2a:

Filename: midsem-q2a.c
Compile : gcc midsem-q2a.c -o midsem-q2a
Execute : ./midsem-q2a [n]
                        n is no.of terms upto which series sum is calculated

Sample session:
$ ./midsem-q2a 7
n = 7
Sum of negative numbers in the series is -0.916667
Sum of positive prime numbers in the series is 0.676190
Sum of positive non-prime numbers in the series is 1.000000
Final result of series is 0.759524

$ ./midsem-q2a 10
n = 10
Sum of negative numbers in the series is -1.141667
Sum of positive prime numbers in the series is 0.676190
Sum of positive non-prime numbers in the series is 1.111111
Final result of series is 0.645635

===========================================================================

Answer 2b:

Filename: midsem-q2b.c
Compile : gcc midsem-q2b.c -pthread -o midsem-q2b
Execute : ./midsem-q2b

Sample session:
$ ./midsem-q2b
Enter number of rows for matrices, m: 2
Enter number of columns for matrices, n: 3

Enter matrix A:
1 2 3
4 5 6

Enter matrix B:
7 8 9 
0 1 2

Enter matrix C:
3 4 5
6 7 8

Checking the correctness of all the four properties of matrix addition...
Commutative

Associative

Additive Identity exists for A and is equal to:
	0	0	0
	0	0	0

Additive Inverse of A exists and is equal to:
	-1	-2	-3
	-4	-5	-6

----------------------------------------------------------------------------
$ ./midsem-q2b
Enter number of rows for matrices, m: 2
Enter number of columns for matrices, n: 2

Enter matrix A:
7 -5
3 0 

Enter matrix B:
1 -2
-5 6

Enter matrix C:
1 0 
0 1

Checking the correctness of all the four properties of matrix addition...
Commutative

Associative

Additive Identity exists for A and is equal to:
	0	0
	0	0

Additive Inverse of A exists and is equal to:
	-7	5
	-3	0

===========================================================================

Answer 3:

Filename: midsem-q3.cpp
Compile : g++ midsem-q3.cpp -o midsem-q3
Execute : ./midsem-q3

Sample session:
$ ./midsem-q3
Number of processes: 7

Enter details for process 1:
Arrival Time: 0
Priority: 2
Burst Time: 4


Enter details for process 2:
Arrival Time: 1
Priority: 4
Burst Time: 2


Enter details for process 3:
Arrival Time: 2
Priority: 6
Burst Time: 3


Enter details for process 4:
Arrival Time: 3
Priority: 10
Burst Time: 5


Enter details for process 5:
Arrival Time: 4
Priority: 8
Burst Time: 1


Enter details for process 6:
Arrival Time: 5
Priority: 12
Burst Time: 4


Enter details for process 7:
Arrival Time: 6  
Priority: 9 
Burst Time: 6


Average Waiting time: 11.43
Average Turnaround time: 15.00
Order of Completion:	P6 P4 P7 P5 P3 P2 P1 

===========================================================================

Answer 4:

Filename: midsem-q4.sh
Execute : ./midsem-q4.sh [F] [K]
		  F is name of a text file
		  K is a number

All the words in file F which occur with frequency greater than K and their 
occurrences from all other files within the directory are replaced with the 
word "MASKED".

Text files used in this program:
	file1.txt	file2.txt	file3.txt
	file4.txt	file5.txt	file6.txt
These text files are also attached

Sample session:
$ ./midsem-q4.sh file1.txt 10
Frequencies of words in file1.txt:
metus. 1
Vivamus 3
metus, 1
condimentum 2
	...
Pellentesque 6
est 1
facilisis 2
	...
mi 5
libero. 1
posuere. 1

Files with frequency of "MASKED" > 5 in current directory:
file1.txt : 46
file4.txt : 46
file5.txt : 45
file6.txt : 41
file3.txt : 40
file2.txt : 36

Note:
In the above output, some of the words have been intentionally skipped because
there are 254 words and showing all makes this readme very lengthy.
===========================================================================