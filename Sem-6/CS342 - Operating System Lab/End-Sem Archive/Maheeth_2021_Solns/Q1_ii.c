/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Endsem
        Question 1 Part (ii)
*/
#include <stdio.h>

// explanation behind the approach is written in README
int main() {
    int n;
    printf("Number of processes (n): ");
    scanf("%d",&n);

    int c[n];       // array to store required copies of the resource by each processes
    int sumC = 0;   // sum of number of required copies of the resource for all processes
    for(int i = 0; i < n; i++) {
        printf("Number of copies for process %d, (c%d): ",i+1,i+1);
        scanf("%d",&c[i]);
        sumC += c[i];
    }

    printf("\nMaximum of %d copies of resource are required to \nnot guarantee deadlock free operation for the system\n",sumC-n);
    printf("\nMinimum of %d copies of resource are required to \nguarantee deadlock free operation for the system\n",sumC-n+1);
    
}