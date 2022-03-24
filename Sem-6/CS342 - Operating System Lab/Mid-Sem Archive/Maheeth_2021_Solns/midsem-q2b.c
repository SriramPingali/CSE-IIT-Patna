/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Midsem Assignent
                Question 2b
    
    Threads
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 10000

// order of matrices A,B,C
int m,n;

// matrices A,B,C
int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

// additive identity and additive inverse matrices of A
int identity[MAX][MAX], inverse[MAX][MAX];

// function to check commutativity
void* commutative(void *args) {
    // check whether A + B = B + A
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(A[i][j] + B[i][j] != B[i][j] + A[i][j]) {
                exit(0);
            }
        }
    }

    // if A + B = B + A, then commutativity holds
    printf("Commutative\n\n");
    pthread_exit(NULL);
}

// function to check associativity
void* associative(void *args) {
    // check whether (A + B) + C = A + (B + C)
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if((A[i][j] + B[i][j]) + C[i][j] != A[i][j] + (B[i][j] + C[i][j])) {
                exit(0);
            }
        }
    }

    // if (A + B) + C = A + (B + C), then associativity holds
    printf("Associative\n\n");
    pthread_exit(NULL);
}

// function to check existence of additive identity of matrix A
void* additiveIdentity(void *args) {
    // additive identity = O (zero matrix)
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            identity[i][j] = 0;
        }
    }

    // check whether A + O = O + A = A
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(A[i][j]+identity[i][j]!=A[i][j] || identity[i][j]+A[i][j]!=A[i][j]) {
                exit(0);
            }
        }
    }

    // if A + O = O + A = A, then O is additive identity
    printf("Additive Identity exists for A and is equal to:\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            printf("\t%d",identity[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    pthread_exit(NULL);
}

// function to check existence of additive inverse of matrix A
void* additiveInverse(void *args) {
    // additive inverse of A = -A
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            inverse[i][j] = -A[i][j];
        }
    }

    // check whether A + (-A) = (-A) + A = O (zero matrix)
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(A[i][j]+inverse[i][j]!=0 || inverse[i][j]+A[i][j]!=0) {
                exit(0);
            }
        }
    }

    // if A + (-A) = (-A) + A = O, then -A is additive inverse of A
    printf("Additive Inverse of A exists and is equal to:\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            printf("\t%d",inverse[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    pthread_exit(NULL);
}

int main() {
    // enter order of matrices A,B,C
    printf("Enter number of rows for matrices, m: ");
    scanf("%d",&m);
    printf("Enter number of columns for matrices, n: ");
    scanf("%d",&n);

    printf("\nEnter matrix A:\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            scanf("%d",&A[i][j]);
        }
    }

    printf("\nEnter matrix B:\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            scanf("%d",&B[i][j]);
        }
    }

    printf("\nEnter matrix C:\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            scanf("%d",&C[i][j]);
        }
    }
    printf("\n");

    printf("Checking the correctness of all the four properties of matrix addition...\n");

    pthread_t comm_thread;  // thread to check commutativity
    int rc = pthread_create(&comm_thread, NULL, commutative, NULL);
    if(rc) {
        printf("Error: Can\'t create thread for checking commutativity\n");
        exit(1);
    }
    usleep(1);

    pthread_t ass_thread;  // thread to check associativity
    rc = pthread_create(&ass_thread, NULL, associative, NULL);
    if(rc) {
        printf("Error: Can\'t create thread for checking associativity\n");
        exit(1);
    }
    usleep(1);

    pthread_t identity_thread;  // thread to check existence of additive identity
    rc = pthread_create(&identity_thread, NULL, additiveIdentity, NULL);
    if(rc) {
        printf("Error: Can\'t create thread for checking existence of additive identity\n");
        exit(1);
    }
    usleep(1);

    pthread_t inverse_thread;  // thread to check existence of additive inverse
    rc = pthread_create(&inverse_thread, NULL, additiveInverse, NULL);
    if(rc) {
        printf("Error: Can\'t create thread for checking existence of additive inverse\n");
        exit(1);
    }
    usleep(1);

    pthread_exit(NULL);
}