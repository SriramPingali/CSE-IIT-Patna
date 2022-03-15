# Smart_Lock_System
Deep Learning based Finite State Machine (FSM) implementation of a Smart Lock System

## SMART LOCK SYSTEM - CS225/ CS226 Switching Theory Project - 2020
Name - Nischal A; Roll No - 1801CS33

The structure of this folder is as follows:-

1. SMART LOCK SYSTEM.pdf is the power point presentation demonstrating the work.
2. Face Recognition folder - contains code, datasets and results on running face recognition through webcam or on videos (of person         trying to open lock)
3. access_grant.dat - contains the value of "valid" - generated from face recognition module. "valid" is taken as an input to the fsm      working.
4. lock_fsm.v is the verilog file containing the code for Lock FSM implementation
5. lock_fsm_tb.v is the corresponding test bench.
6. read_valid.v is a Verilog program to read data from the acces_grant.dat file (which contains "valid" value after face recognition       is run)
