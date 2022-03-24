CS342 - Operating System Lab

Assignment 3
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            02-Feb-2021
======================================

Answer 1:

Filename: ques1.c
Compile: gcc ques1.c -o ques1
Command to execute: ./ques1 [integer-1] [integer-2]

Example:
Command: ./ques1 2 3
Output:
Sum by Child 16150 : 5
Multiplication by Child 16151 : 6
Division by Parent 12623 : 1.200000
----------------------------------------------

Command: ./ques1 192 123
Output:
Sum by Child 16176 : 315
Multiplication by Child 16177 : 23616
Division by Parent 12623 : 74.971428
==============================================

Answer 2:

Filename: ques2.c
Compile: gcc ques2.c -o ques2
Command to execute: ./ques2
Output:
HelloHelloHelloHelloHello
==============================================

Answer 3:

Filename: ques3.c
Compile: gcc ques3.c -o ques3
Command to execute: ./ques3 <args for task1.sh> <args for task2.sh>
            ==> ./ques3 [N for task1] [n1 n2 n3 ... nN] [S] [N for task2]

Example:
Command: ./ques3 5 2 1 3 4 5 6 7

Explanation:
N for task1 is 5
n1 n2 n3 n4 n5 is 2 1 3 4 5
S is 6
N for task2 is 7

Output:
N = 5
n has 5 items: 2 1 3 4 5
You are searching for 6
Item not found

Factorial of 7 = 5040
Sum of Primes less than 7 = 17
Output = 85680
----------------------------------------------

Command: ./ques3 5 2 1 3 4 5 5 9

Explanation:
N for task1 is 5
n1 n2 n3 n4 n5 is 2 1 3 4 5
S is 5
N for task2 is 9

Output:
N = 5
n has 5 items: 2 1 3 4 5
You are searching for 5
Search found searched item S=5 at index=5

Factorial of 9 = 362880
Sum of Primes less than 9 = 17
Output = 6168960
===================================================================