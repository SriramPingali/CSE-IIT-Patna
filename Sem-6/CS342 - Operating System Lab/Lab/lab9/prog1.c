/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Assignment 9
                Question 1
    
    DEADLOCK DETECTION
*/
#include <stdio.h>
#define MAX 100

// no. of processes and resources
int P, R;

// matrices for max allocation, current allocation. And need matrix
int max[MAX][MAX], alloc[MAX][MAX], need[MAX][MAX];

// availablity resources
int avail[MAX];

// array to check if a process is finished or not
int finish[MAX] = {0};

// boolean variable to mark end of execution for given inputs
int NEOE = 0;

// function to perform bankers algorithm
void bankers() {
    while (1) {
        NEOE = 0;
        for (int i = 0; i < P; i++) {
            // if a process is not executed and has enough resources, execute it
            if (!finish[i]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (need[i][j] > avail[j]) break;
                }

                if (j == R) {
                    for (j = 0; j < R; j++) {
                        avail[j] += alloc[i][j];
                    }

                    NEOE = 1;
                    finish[i] = 1;
                }
            }
        }
        if (NEOE == 0) break;
    }
}

int main() {
    printf("SIMULATION OF DEADLOCK PREVENTION\n");

    // inputs
    printf("Enter number of processes, resources: ");
    scanf("%d %d",&P,&R);

    printf("Enter allocation matrix\n");
    for (int j = 0; j < R; j++) {
        for (int i = 0; i < P; i++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    printf("\nEnter max matrix\n");
    for (int j = 0; j < R; j++) {
        for (int i = 0; i < P; i++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("\nEnter availablity matrix\n");
    for (int i = 0; i < R; i++) {
        scanf("%d", &avail[i]);
    }

    // need matrix
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // perform bankers algo for given inputs
    bankers();
    
    printf("\n-------------\n");
    
    // if all processes are executed, no deadlock is there
    if (NEOE == 0) {
        int sum = 0;
        for (int i = 0; i < P; i++) {
            sum += finish[i];
        }
        if(sum == P) {
            printf("There is no deadlock\n");
            return 0;
        }
    }

    // avoiding Mutual exclusion, preemption, hold and wait can prevent deadlock
    for (int i = 0; i < P; i++) {
        if (NEOE == 0) {
            if (!finish[i]) {
                // if any process was not executed because of insufficient resources, check if execution is possible with sufficient resources
                printf("- Failing: Mutual exclusion: ");
                for (int j = 0; j < R; j++) {
                    if (avail[j] < need[i][j]) {
                        avail[j] = need[i][j];
                    }
                }
                bankers();
                if (finish[i] == 1) {
                    printf("By allocating required resources to process %d deadlock is prevented\n", i);
                }
                
                // similarly if any process was not executed because of preemption, check if execution is possible with necessary conditions for execution
                printf("- Lack of Preemption\n");
                for (int j = 0; j < R; j++) {
                    if (avail[j] < need[i][j]) {
                        avail[j] = need[i][j];
                    }
                    alloc[i][j] = 0;
                }
                bankers();
                if (finish[i] == 1) {
                    printf("- Deadlock can be prevented by allocating needed resources\n");
                }

                // do the same for hold and wait condition
                printf("- Failing: Hold and Wait condition\n");
                for (int j = 0; j < R; j++) {
                    if (avail[j] < need[i][j]) {
                        avail[j] = need[i][j];
                    }
                }
                bankers();
            }
        }
    }
}
