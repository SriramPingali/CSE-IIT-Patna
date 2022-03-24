CS342 - Operating System Lab

Assignment 2
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            19-Jan-2021
======================================

Answer 1:

Filename: ans1.sh
Command to execute: ./ans1.sh [N] [n1 n2 n3 ... nN] [S]

Example:
Command: ./ans1.sh 10 5 6 1 2 9 0 5 2 4 10 10
Output:
N = 10
n has 10 items: 5 6 1 2 9 0 5 2 4 10
You are searching for 10
Search found searched item S=10 at index=10
----------------------------------------------

Command: ./ans1.sh 4 5 6 1 2 9 0 5 2 4 7
Output:
Error: Too many arguments!
----------------------------------------------

Command: ./ans1.sh 9 5 6 1 2 9 0 5 2 4 10                                                                                                                                23:56:41
Output:
N = 9
n has 9 items: 5 6 1 2 9 0 5 2 4
You are searching for 10
Item not found
==============================================

Answer 2:

Filename: ans2.sh
Command to execute: ./ans2.sh [n]

Example:
Command: ./ans2.sh 8
Output:
Factorial of 8 = 40320
Sum of Primes less than 8 = 17
Output = 685440
----------------------------------------------

Command: ./ans2.sh 10
Output:
Factorial of 10 = 3628800
Sum of Primes less than 10 = 17
Output = 61689600
----------------------------------------------

Command: ./ans2.sh -10
Output:
Error: n must be positve!
==============================================

Answer 3:

For Bash Script version
-----------------------
Filename: ans3.sh
Command to execute: time ./ans3.sh [number-1] [number-2]

Example:
Command: time ./ans3.sh 81 100
Output:
181 in binary: 10110101
101 in binary: 01100101
XOR of 10110101 and 01100101 is 11010000
11010000 in decimal is 208

real	0m0.007s
user	0m0.001s
sys	    0m0.006s

For Python 3 script version
---------------------------
Filename: ans3-python.py
Command to execute: time python3 ans3-python.py [number-1] [number-2]

Example:
Command: time python3 ans3-python.py 81 100
Output:
181 in binary: 10110101
101 in binary: 01100101
XOR of 10110101 01100101 is 11010000
11010000 in decimal is 208

real	0m0.031s
user	0m0.028s
sys	    0m0.004s


Observation:
It takes lesser time to execute the Bash script (time: 0.007s)than
the equivalent Python 3 script (time: 0.032s) for the same inputs.
===================================================================

Answer 4:

Filename: ans4.sh
Command to execute: ./ans4.sh

Examples:

Command: ./ans4.sh
Output:
Enter Password: maheeth123/
Valid Password
------------------------------

Command: ./ans4.sh
Output:
Enter Password: abracadabra
Invalid Password
------------------------------

Command: ./ans4.sh
Output:
Enter Password: 12345
Invalid Password
==============================