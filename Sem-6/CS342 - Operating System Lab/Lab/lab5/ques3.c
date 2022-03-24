#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#define NUM 1000

// array to store cube roots
double* result;

// function to calculate cubes
void* calcCubes(void* args) {
	result = (double *)malloc(sizeof(double) * NUM);

	for(int i = 0; i < NUM; i++) {
		result[i] = cbrt((double)i);
	}
	
	return (void *)result;
}

int main() {
	pthread_t TID;
	void* exit_status;
	
	// Create thread for calculating cubes
	pthread_create(&TID, NULL, calcCubes, NULL);
	pthread_join(TID, &exit_status);

	printf("Computation under process. Please wait...\n");

	// display cube roots
	for(int i = 0; i < NUM; i++) {
		printf("cbrt(%d) = %lf\n", i, result[i]);
	}
	free(result);

	return 0;
}