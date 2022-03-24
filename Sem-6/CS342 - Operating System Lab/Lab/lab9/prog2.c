/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Assignment 9
                Question 2
    
    DEADLOCK DETECTION
*/
#include <stdio.h>
#define MAX 100

// no. of processes and resources
int P,R;

// total resources
int total[MAX];

// availablity resources
int work[MAX];

// matrices for max allocation, current allocation. And need matrix
int request[MAX][MAX], alloc[MAX][MAX], need[MAX][MAX];

// array to check if a process is finished or not
int finish[MAX] = { 0 };

void printSafeSeq(int safeSeq[]) {
    for(int i = 0; i < P; i++) {
        printf("P%d", safeSeq[i]);
        if(i == P-1) printf("\n");
        else printf(" -> ");
    }
}

int suitableProcess() {
    for(int i = 0; i < P; i++) {
        if(!finish[i]) {
            int flag = 1;
            for(int j = 0; j < R; j++) {
                if(request[i][j] > work[j]) {
                    flag = 0;
                }
            }
            if(flag) return i;
        }
    }
    return -1;
}

int countExec() {
    int exec = 0;
    for(int i = 0; i < P; i++) exec += finish[i];
    return exec;
}

int main() {
    printf("Enter no. of processes: ");
    scanf("%d",&P);

    printf("Enter no. of resources: ");
    scanf("%d",&R);

    printf("\n");
    for(int i = 0; i < R; i++) {
        printf("Enter Total Amount of Resource ");
        printf("R%d: ",i+1);
        scanf("%d",&total[i]);
    }

    printf("\nEnter the request matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("P%d: ",i);
        for(int j = 0; j < R; j++) {
            scanf("%d",&request[i][j]);
        }
    }

    printf("\nEnter the allocation matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("P%d: ",i);
        for(int j = 0; j < R; j++) {
            scanf("%d",&alloc[i][j]);
        }
    }

    // calculate available resources
    for(int r = 0; r < R; r++) {
        int Ralloc = 0;
        for(int p = 0; p < P; p++) Ralloc += alloc[p][r];

        work[r] = total[r] - Ralloc;
    }

    int order[P], index = 0;
    for(int i = 0; i < P; i++) order[i] = -1;

    for(int i = 0; i < P; i++) {
        int req = 0;
        for(int j = 0; j < R; j++) {
            req += request[i][j];
        }
        if(req == 0) {
            finish[i] = 1;
            order[index++] = i;
        }
    }

    while(countExec() < P) {
        int proc = suitableProcess();
        if(proc == -1) break;

        for(int i = 0; i < R; i++) {
            work[i] += alloc[proc][i];
        }
        finish[proc] = 1;
        order[index++] = proc;
    }

    for(int i = 0; i < P; i++) {
        if(!finish[i]) {
            printf("Deadlock detected\n");
            printf("Process P%d is deadlocked\n",i);
            return 0;
        }
    }

    printf("There is no deadlock here\n");
    printf("Possible Safe sequence:\t");
    printSafeSeq(order);
}