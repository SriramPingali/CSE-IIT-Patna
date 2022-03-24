// OS Lab - Assignment 8
// Name of Student: M. Maheeth Reddy
// Roll No.:        1801CS31

// prog2.c - Sum of primes from 0 to 10000 by 4 threads
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 4
#define QUARTER 2500

// store thread ids
pthread_t thread_id[MAX_THREADS];   
int sumPrimes = 0;
pthread_mutex_t mtx;

// structure to send arguments to thread
typedef struct args {
    int tid, begin;
} args;

// function to check if a number is prime or not
int isPrime(int num) {
    if (num <= 1) return 0;
    
    int end = num/2;
    for(int i = 2; i <= end; i++) {
        if(num%i == 0) return 0;
        else end=num/i;
    }
    return 1;
}

// calculate sum of primes in a quarter of the range
void* sumFun(void* params) {
    pthread_mutex_lock(&mtx);

    args* prm = (args*) params;

    int start = prm->begin;
    int end = start + QUARTER;
    
    int curPrimes = 0;
    for (int num = start; num < end; num++) {
        if(isPrime(num) == 1) {
            sumPrimes += num;
            curPrimes += num;
        }
    }
    
    printf("Sum of primes in thread %d: %d\n", prm->tid, curPrimes);
    pthread_mutex_unlock(&mtx);
	return NULL;
}

int main() {
    // starting numbers for each quarter of the range
    args **q = (args**) malloc(MAX_THREADS * sizeof(args*));
    for(int i = 0; i < MAX_THREADS; i++) {
        q[i] = (args*) malloc(sizeof(args));
        
        // one argument for each thread is thread serial number (0,1,2,3)
        q[i]->tid = i;

        // second argument is starting number for corresponding quarter
        q[i]->begin = i*QUARTER;
    }

    int rc;
    // create threads to calculate sum of primes in each quarter
    for(int i = 0; i < MAX_THREADS; i++) {
        rc = pthread_create(&thread_id[i], NULL, sumFun, (void*) q[i]);

        if (rc != 0){
		    printf("\nThread %d can't be created :[%s]",i,strerror(rc));
            exit(1);
	    }
    }

    // join all threads
	for(int i = 0; i < MAX_THREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    printf("Sum of all primes from 0 to 10000 is %d\n", sumPrimes);
    
    // free heap variables
    for(int i = 0; i < MAX_THREADS; i++) {
        free(q[i]);
    }
    free(q);

    pthread_exit(NULL);
}