#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Convert command line arguments to integers
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);

    // Variables for sum,product and division
    int sum, prod;
    float div;

    // File descriptors
	int fd1[2],fd2[2];
	pipe(fd1);
	pipe(fd2);

    // PIDs for forked processes
	pid_t pid1, pid2;

	pid1 = fork();
    // child process-1
	if(pid1 == 0) {
	    close(fd1[0]);

		sum=x+y;
		printf("Sum by Child %d : %d\n", getpid(), sum);

        // send sum to parent process
		write(fd1[1], &sum, sizeof(sum));
        close(fd1[1]);
	} else {
	    pid2 = fork();
        // child process-2
	    if (pid2 == 0) {
	    	close(fd2[0]);

	        prod=x*y;
	        printf("Multiplication by Child %d : %d\n", getpid(), prod);

            // send prod to parent process
	        write(fd2[1], &prod, sizeof(prod));
        	close(fd2[1]);
	    } else {
	        // Parent Code

            // receive sum from child-1
	        close(fd1[1]);
	        read(fd1[0], &sum, sizeof(sum));
	        close(fd1[0]);

            // receive prod from child-2
	        close(fd2[1]);
	        read(fd2[0], &prod, sizeof(prod));
	        close(fd2[0]);

            // Print final result
	   		div=prod*1.0/sum;
	   		printf("Division by Parent %d : %f\n", getppid(), div);
	    }
	}
    
	return 0;
}