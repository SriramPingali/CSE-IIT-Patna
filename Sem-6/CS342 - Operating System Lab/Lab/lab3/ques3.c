#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 100

// Assumption: Command line Arguments for task1.sh should be provided
//             followed by arguments for task2.sh

int main(int argc, char *argv[]) {
    // PID of forked process (executes task1.sh)
    int pid;
    pid = fork();

    // Command Line Arguments for task1.sh
    char* args1[MAX];
    args1[0] = "bash";
    args1[1] = "task1.sh";
    // Copy remaining arguments to args1 array
    for(int i = 1; i < argc; i++) {
        args1[i+1] = argv[i];
    }
    args1[argc+1] = NULL;

    // Command Line Arguments for task2.sh
    char* args2[MAX] = {"bash", "task2.sh", argv[argc-1], NULL};

    // parent executes task2.sh, child executes task1.sh
    if(pid == 0) {
        // task1.sh
        execv("/bin/bash",args1);
    } else {
        // task2.sh
        execv("/bin/bash",args2);
    }

    return 0;
}