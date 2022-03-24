#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAX_THREADS 50

pthread_t TID[MAX_THREADS];   

// print hello message from thread
void* hello(void* data) {
    printf("Hello from groot %u - I was created in iteration %d\n", (int)pthread_self(), (int)data);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Mention the number of threads as command line argument\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    if(n > MAX_THREADS) n = MAX_THREADS;

    int rc;
    for(int i = 0; i < n; i++) {
        rc = pthread_create(&TID[i], NULL, hello, (void*)i);
        if(rc) {
            printf("ERROR: return code from pthread_create is %d\n", rc);
            exit(1);
        }
        printf("I am groot %u. Created new thread (%u) in iteration %d...\n", (int)pthread_self(), (int)TID[i], i);
        if(i % 5 == 0) sleep(1);
    }

    pthread_exit(NULL);
}