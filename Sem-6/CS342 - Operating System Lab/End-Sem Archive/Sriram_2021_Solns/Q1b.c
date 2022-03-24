#include <stdio.h>

// Utility function to find
// Max number of processes non-guarantee deadlock free
int max_guarantee(int n, int* c)
{
	int max = 0;

	// Find Sum(ci) - n;
	for (int i = 0; i < n; i++)
		max += c[i];
	max -= n;
	return(max);
}

// Driver Code
int main()
{
	// Number of processes
	int n;

	printf("Enter the number of processes: ");
	scanf("%d", &n);

	// Array of requirements
	int c[n];
	printf("Enter the requirements of processes: ");
	for (int i = 0; i < n; i++)
		scanf("%d", &c[i]);

	// Find the required values
	int max = max_guarantee(n, c);
	int min = max + 1;

	printf("Maximum resources for non guaranteed deadlock free operation: %d\n", max);
	printf("Minimum resources for guaranteed deadlock free operation: %d\n", min);
}