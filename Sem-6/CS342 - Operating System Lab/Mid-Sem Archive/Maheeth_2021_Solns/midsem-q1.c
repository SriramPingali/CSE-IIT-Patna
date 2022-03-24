/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Midsem Assignent
                Question 1
    
    C program to execute Zombie and Orphan process in a single program
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Parent process is forked\n");

    int pid1 = fork();
    if(pid1 == 0) {
        // Child process 1 quits immediately
        printf("This is Child Process 1: PID=%d\tParent PID=%d\n", getpid(), getppid());
        printf("Child process 1 is quitting\n");
        exit(0);
    } else {
        // Now parent process is zombie process
        sleep(5);
        printf("This is the Parent Process: PID=%d\n", getpid());
        printf("Parent Process is a ZOMBIE process now\n\n");

        
        printf("Parent process is forked once again\n");
        int pid2 = fork();
        if(pid2 > 0) {
            // Parent process quits
            printf("Parent Process (PID=%d) is quitting\n", getpid());
            exit(0);
        } else {
            // Child process 2 becomes orphan process
            printf("This is Child Process 2: PID=%d\tParent PID=%d\n", getpid(), getppid());
            printf("Child Process 2 is an ORPHAN process now\n");
            sleep(5);
        }
    }
}