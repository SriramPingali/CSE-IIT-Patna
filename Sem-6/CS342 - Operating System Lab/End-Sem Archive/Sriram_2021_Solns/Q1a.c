#include <stdio.h>

// Driver Code
int main()
{
	// Variable to store resources, requirements
	int r, c;

	printf("Enter the resources required by each process (c): ");
	scanf("%d", &c);

	printf("Enter the total number of resources available (r): ");
	scanf("%d", &r);

	// If c = 1, unlimited resources could be satisfied 
	if(c == 1 && r >= 1)
		printf("Can give Unlimited number of processes\n");
	// If any negative value => Invalid
	else if(c <= 0 || r <= 0)
		printf("Invalid Input\n");
	// If normal case, formula = (r - 1) / (c - 1)
	else
		printf("Maximum number of resources to ensure deadlock free scenario(n): %d\n", (r - 1) / (c - 1));
}