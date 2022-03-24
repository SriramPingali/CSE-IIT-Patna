// OS Lab - Assignment 8
// Name of Student: M. Maheeth Reddy
// Roll No.:        1801CS31

// prog3.c - Dining Philosopher's Problem
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5

#define EAT_STATE 0
#define HUNGRY_STATE 1
#define THINK_STATE 2

#define LEFT (phil+4)%N
#define RIGHT (phil+1)%N

int state[N] = { THINK_STATE };
int phil[N] = { 0, 1, 2, 3, 4 };

int allocation[N][N];
int request[N][N];

sem_t mutex;
sem_t S[N];

void printMatrix(int mat[][N]) {
	for (int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void test(int phil) {
	if (state[phil] == HUNGRY_STATE && state[LEFT] == THINK_STATE && state[RIGHT] != EAT_STATE) {
		state[phil] = EAT_STATE;
		sleep(2);

		printf("Philosopher %d grabs fork %d and %d\n", phil+1, LEFT+1, phil+1);
		
		allocation[phil][LEFT] = 1;
		allocation[phil][phil] = 1;

		printf("Allocation Matrix:\n");
		printMatrix(allocation);

		printf("Philosopher %d starts eating\n", phil + 1);
		sem_post(&S[phil]);
	}
}

// check whether deadlock occurs
int checkDeadlock() {
	int flag_1 = 1, flag_2 = 1;
	
	for(int i = 0; i < N; i++) {
		if(request[i][i] == 0) flag_1 = 0;
		if(request[i][i-1] == 0) flag_2 = 0;
	}
	
	if(flag_1 == 1 || flag_2 == 1) return 1;

	return 0;
}

// Philospher ends eating and releases the forks
void dropFork(int phil) {
	sem_wait(&mutex);

	// philosopher is thinking
	state[phil] = THINK_STATE;

	printf("Philosopher %d ends eating and releases forks %d (left) and %d (right) down\n", phil + 1, LEFT + 1, phil + 1);

	allocation[phil][LEFT] = 0;
	allocation[phil][phil] = 0;

	printf("Allocation Matrix:\n");
	printMatrix(allocation);

	printf("Philosopher %d starts thinking\n", phil + 1);

	request[phil][LEFT] = 0;
	request[phil][phil] = 0;

	printf("Request Matrix:\n");
	printMatrix(request);

	test(LEFT);
    test(RIGHT);

	sem_post(&mutex);
}

// Philospher picks up the forks
void pickFork(int phil) {
	// philosopher is hungry
	state[phil] = HUNGRY_STATE;
	sleep(1);

	sem_wait(&mutex);
	printf("Philosopher %d is hungry\n", phil + 1);

	request[phil][LEFT] = 1;
	request[phil][phil] = 1;

	printf("Request Matrix:\n");
	printMatrix(request);

	if(checkDeadlock()) {
		printf("A deadlock has been detected and recovery begins\n\n");
		sleep(5);
		request[phil][LEFT] = 0;
		
		sem_post(&mutex);
		return;
	}

	// philosopher eats only if neighbours are not eating
	test(phil);

	sem_post(&mutex);

	// if philosopher cannot eat, wait to be signalled
	sem_wait(&S[phil]);

	sleep(1);
}

// function to emulate philosopher
void* philospher(void* num) {
	int* i = num;
	sleep(1);
	pickFork(*i);
	sleep(0);
	dropFork(*i);
}

int main() {
	bzero(allocation, sizeof(allocation));
	bzero(request, sizeof(request));

	pthread_t thread_id[N];

	// initialize the semaphores
	sem_init(&mutex, 0, 1);

	int i;
	for(i = 0; i < N; i++) {
		sem_init(&S[i], 0, 0);
	}

	// create philosopher processes
	for(i = 0; i < N; i++) {
		pthread_create(&thread_id[i], NULL, philospher, &phil[i]);
		printf("Philosopher %d starts thinking\n", i+1);
	}

	for(i = 0; i < N; i++) {
		pthread_join(thread_id[i], NULL);
	}
}
