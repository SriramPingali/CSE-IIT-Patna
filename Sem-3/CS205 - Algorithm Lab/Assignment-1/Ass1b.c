#include <stdio.h>

int numOfWays(int totalSteps, int steps[], int stepIndex, int currentStep) {
	if(currentStep > totalSteps) return 0;

	if(currentStep == totalSteps) {
		for(int i = 0; i < stepIndex; i++) {
			printf("%d",steps[i]);
			if(i != stepIndex-1) printf(",");
			else printf("\n");
		}
		return 1;
	}

	int noOfWays = 0;
	for(int i = 1; i <= 3; i++)
	{
		steps[stepIndex] = i;
		noOfWays += numOfWays(totalSteps,steps,stepIndex+1,currentStep+i);
	}
	
	return noOfWays;
}

int main() {
	int totalSteps, steps[100], stepIndex = 0, currentStep = 0;
	printf("Total no. of steps = ");
	scanf("%d",&totalSteps);
	for(int i = 0; i < 100; i++) steps[i] = 0;
	int tot = numOfWays(totalSteps,steps,stepIndex,currentStep);
	printf("Total number of ways = %d\n", tot);
}