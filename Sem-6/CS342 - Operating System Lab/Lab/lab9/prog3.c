/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Assignment 9
                Question 3
    
    DEADLOCK AVOIDANCE (BANKER’S ALGORITHM)
*/
#include <stdio.h>
#define MAX 100

// no. of processes and resources
int P,R;

// total resources
int total[MAX];

// availablity resources
int avail[MAX];

// matrices for max allocation, current allocation. And need matrix
int max[MAX][MAX], alloc[MAX][MAX], need[MAX][MAX];

// array to check if a process is executed or not
int executed[MAX] = { 0 };

// check whether a process has sufficient resources for execution
int canExecProcess(int process) {
    for(int i = 0; i < R; i++) {
        if(avail[i] < need[process][i]) {
            return 0;
        }
    }

    return 1;
}

// size of safe sequence formed so far
int safeSeqSize(int safeSeq[]) {
    int size = 0;
    while(size < P) {
        if(safeSeq[size] == -1) break;
        else size++;
    }
    return size;
}

void printSafeSeq(int safeSeq[]) {
    for(int i = 0; i < P; i++) {
        printf("P%d", safeSeq[i]);
        if(i == P-1) printf("\n");
        else printf(" -> ");
    }
}

// find all safe sequences
void findSafeSeqs(int safeSeq[], /*int index,*/ int* numSeq) {
    // find all safe sequences possible beginning with P0,P1,P2,...
    for(int i = 0; i < P; i++) {
        if(!executed[i] && canExecProcess(i)) {
            executed[i] = 1;

            for(int j = 0; j < R; j++) {
                avail[j] += alloc[i][j];
            }

            int index = safeSeqSize(safeSeq);
            safeSeq[index] = i;
            findSafeSeqs(safeSeq, /*index+1,*/ numSeq);
            safeSeq[index] = -1;

            executed[i] = 0;

            for(int j = 0; j < R; j++) {
                avail[j] -= alloc[i][j];
            }
        }
    }

    // if safe sequence is formed print it
    if(safeSeqSize(safeSeq) == P) {
        *numSeq = *numSeq + 1;
        printSafeSeq(safeSeq);
    }

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

    printf("\nEnter the max matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("P%d: ",i);
        for(int j = 0; j < R; j++) {
            scanf("%d",&max[i][j]);
        }
    }

    printf("\nEnter the allocation matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("P%d: ",i);
        for(int j = 0; j < R; j++) {
            scanf("%d",&alloc[i][j]);
        }
    }

    // calculate availability matrix
    for(int r = 0; r < R; r++) {
        int Ralloc = 0;
        for(int p = 0; p < P; p++) Ralloc += alloc[p][r];

        avail[r] = total[r] - Ralloc;
    }

    // calculate need matrix
    for(int i = 0; i < P; i++) {
        for(int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // no. of safe sequences
    int numSeq = 0;

    // store safe sequences
    int safeSeq[MAX], index = 0;
    for(int i = 0; i < MAX; i++) safeSeq[i] = -1;
    
    printf("\nList of possible safe sequences:\n");
    findSafeSeqs(safeSeq, /*index,*/ &numSeq);
    printf("Total number of safe sequences: %d\n",numSeq);
}