CS342 - Operating System Lab

Assignment 1
------------
Name of Student: M. Maheeth Reddy
Roll No.:		 1801CS31
======================================

Answer 1:

Filename: solution-1.sh
Command to Execute: ./solution-1.sh [integer-number]

Example Outputs:

Command: ./solution-1.sh 324
Output:
Reverse of 324 is 423
----------------------------------
Command: ./solution-1.sh
Output:
Error: No Arguments provided
----------------------------------
Command: ./solution-1.sh -324
Output:
Reverse of -324 is -423
===================================


Answer 2:

Filename: solution-2.sh
Command to execute: ./solution-2.sh [start-number] [end-number]

Example Outputs:

Command: ./solution-2.sh 4 11
Output:
4 5 6 7 8 9 10 11 
----------------------------------
Command: ./solution-2.sh 11 4
Output:
Error: First argument must be lesser than or equal to second argument
----------------------------------
Command: ./solution-2.sh 11 11
Output:
11 
===================================


Answer 3:

Filename: solution-3.sh
Command to execute: ./solution-3.sh [path-to-file] [minimum-number-of-lines-in-a-file]

Example Output:
Command: ./solution-3.sh ~/.bashrc 100
Output:
/home/maheeth/.bashrc has 129 lines
-----------------------------------
Command: ./solution-3.sh ~/.profile 100
/home/maheeth/.profile has 26 lines
Removed /home/maheeth/.profile
===================================


Answer 4:

Filename: solution-4.sh
Command to execute: ./solution-4.sh [directory_path] [filename_pattern] [new_filename]

Example Output:
Command: ./solution-4.sh /home/maheeth/Documents/folder/fold home sam
Output:
arg1:	/home/maheeth/Documents/folder/fold
arg2:	home
arg3:	sam
=========
path:	/home/maheeth/Documents/folder/fold/sam_1.txt
file:	sam_1.txt
---------
path:	/home/maheeth/Documents/folder/fold/sam_2.mp4
file:	sam_2.mp4
---------
path:	/home/maheeth/Documents/folder/fold/sam_3.abc
file:	sam_3.abc
---------
path:	/home/maheeth/Documents/folder/fold/sam_4.xyz
file:	sam_4.xyz
---------

