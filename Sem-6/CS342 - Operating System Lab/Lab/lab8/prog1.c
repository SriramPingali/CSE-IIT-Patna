// OS Lab - Assignment 8
// Name of Student: M. Maheeth Reddy
// Roll No.:        1801CS31

// prog1.c - Applying mutex lock on function
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>

// store thread ids
pthread_t thread_id[2];

// used to see which thread is active, 1 or 2
int tid = 0;

// mutex to protect function f()
pthread_mutex_t mtx;

// function f()
void f(int tid) {
    if(tid == 1){
		printf("pthread1 is accessing function f()\n");
		printf("pthread2 cannot access function f() lock acquired by pthread1\n");
	} else {
		printf("pthread2 is accessing function f()\n");
	}
}

// function to execute particular thread
void* execThread(void* arg) {
	pthread_mutex_lock(&mtx);

	tid += 1;
	printf("Lock acquired by pthread%d\n", tid);

	f(tid);
	sleep(3);

	printf("Lock released by pthread%d\n", tid);
	pthread_mutex_unlock(&mtx);
	return NULL;
}

int main() {
	if (pthread_mutex_init(&mtx, NULL) != 0) {
		printf("\n Couldn't init mutex\n");
		return 1;
	}

	// create thread 1
	int err = pthread_create(&thread_id[0],NULL,&execThread, NULL);
	if (err != 0){
		printf("\nThread can't be created :[%s]",strerror(err));
	}

	// create thread 2
	err = pthread_create(&thread_id[1],NULL,&execThread, NULL);
	if (err != 0){
		printf("\nThread can't be created :[%s]",strerror(err));
		exit(1);
	}

	// join threads
	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	
	pthread_mutex_destroy(&mtx);

	return 0;
}
