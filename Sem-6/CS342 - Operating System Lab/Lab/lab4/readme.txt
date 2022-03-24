CS342 - Operating System Lab

Assignment 4
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
Date:            09-Feb-2021
======================================

Answer 1:

Filename: ques1.c
Compile: gcc ques1.c
Sample session:

$ ./a.out &
[1] 18893

$ pstree -p 18893
a.out(18893)───a.out(18894)

$ ps aux | grep 18894
maheeth  18894  0.0  0.0      0     0 pts/3    Z    15:55   0:00 [a.out] <defunct>
maheeth  18897  0.0  0.0  14432  1076 pts/3    S+   15:56   0:00 grep --color=auto 18894

Observation:
PID of parent process is 18893 and PID of child process is 18894.
The child process exits before parent process begins its execution. In the 
first line of the output for "ps aux | grep 18894", status "Z" indicates 
that the child process (PID=18894) is a zombie process. Hence, the child
process has become a zombie process.

===========================================================================

Answer 2:

Filename: ques2.c
Compile: gcc ques2.c

Sample Session:
$ ./a.out

$ ps aux | grep a.out
maheeth  28035  0.0  0.0   4380    72 pts/3    S    19:28   0:00 ./a.out
maheeth  28039  0.0  0.0  14432  1096 pts/3    S+   19:28   0:00 grep --color=auto a.out

$ pstree -s 28035
systemd───a.out

Observation:
PID of child process is 28035.
The parent process exits before child process begins its execution. In the 
first line of the output for "ps aux | grep a.out", status "S" indicates 
that the child process (PID=28035) is an orphan process.

Significance of an Orphan Process:
Orphan processes are usually used to allow a long-running job to complete 
without further user attention, or to start an indefinitely running service 
or agent. Such processes are known as daemons.

==============================================

Answer 3:

Filename: ques3.c
Compile: gcc ques3.c
Command to execute: ./a.out

Examples:

Command: ./a.out
Output:
Give length of Fibonacci series: 40
Series of first 40 Fibonacci numbers
0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986
Prime Fibonacci numbers are: 
2 at position 4
3 at position 5
5 at position 6
13 at position 8
89 at position 12
233 at position 14
1597 at position 18
28657 at position 24
514229 at position 30

----------------------------------------------

Command: ./a.out
Output:
Give length of Fibonacci series: 10
Series of first 10 Fibonacci numbers
0, 1, 1, 2, 3, 5, 8, 13, 21, 34
Prime Fibonacci numbers are: 
2 at position 4
3 at position 5
5 at position 6
13 at position 8
===================================================================

Answer 4:

Contents of file are treated as strings and are sorted in reverse order
according to the ASCII Table.

Filename: ques4.c
Compile: gcc ques4.c
Command to execute: ./a.out

Examples:

Command: ./a.out
Output:
Child1	PID=29821 Parent-ID=29820
Copying contents of file1.txt to file2.txt
------------------------------------------

Child2	PID=29822 Parent-ID=29820
Displaying contents of file2.txt
--------------------------------
zebra dog buffalo xylophone fish apple giraffe

Child3	PID=29820 Parent-ID=18840
Displaying Sorted Content of file2 in reverse order
---------------------------------------------------
zebra xylophone giraffe fish dog buffalo apple 

-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

Command: ./a.out
Output:
Child1	PID=29789 Parent-ID=29788
Copying contents of file1.txt to file2.txt
------------------------------------------

Child2	PID=29790 Parent-ID=29788
Displaying contents of file2.txt
--------------------------------
Lorem ipsum dolor 32 sit amet, consectetur adipiscing elit. Donec massa tellus, dictum nec arcu et @#! , mattis maximus augue. Proin dignissim odio tempus turpis 54 cursus dapibus ut id erat. Nunc facilisis mauris a quam cursus, id rutrum elit faucibus. In nec nibh mauris. Praesent congue rutrum varius. Ut feugiat viverra} 0$12 dui ut vehicula. Vestibulum at commodo felis. Donec lobortis erat lobortis su88--*scipit vulputate. Nulla volutpat a nibh vel ultrices. Vivamus [237/] augue elit, tristique a tortor sit amet, lacinia vulputate purus. Sed vel 12@agna interdum, dictum magna ut, dapibus elit. Donec accumsan semper mi, ut viverra ex dictum et.

Child3	PID=29788 Parent-ID=18840
Displaying Sorted Content of file2 in reverse order
---------------------------------------------------
vulputate. vulputate volutpat viverra} viverra vel vel vehicula. varius. ut, ut ut ut ultrices. turpis tristique tortor tempus tellus, su88--*scipit sit sit semper rutrum rutrum quam purus. odio nibh nibh nec nec mi, maximus mauris. mauris mattis massa magna lobortis lobortis lacinia ipsum interdum, id id feugiat felis. faucibus. facilisis ex et. et erat. erat elit. elit. elit, elit dui dolor dignissim dictum dictum dictum dapibus dapibus cursus, cursus consectetur congue commodo augue. augue at arcu amet, amet, adipiscing accumsan a a a [237/] Vivamus Vestibulum Ut Sed Proin Praesent Nunc Nulla Lorem In Donec Donec Donec @#! 54 32 12@agna 0$12 , 

===================================================================