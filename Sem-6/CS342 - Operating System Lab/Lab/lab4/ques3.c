#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// function to check whether a number is prime
int isPrime(int num) {
    if (num <= 1) return 0;
    
    int end = num/2;
    for(int i = 2; i <= end; i++) {
        if(num%i == 0) return 0;
        else end=num/i;
    }
    return 1;
}

int main() {
    int n;
    printf("Give length of Fibonacci series: ");
    scanf("%d",&n);

    // pipe between parent and child process
    int fd[2];
    pipe(fd);

    int pid=fork();

    if(pid!=0) {
        // Parent process
        int fnum;
        close(fd[1]);

        // Get the fibonacci series from child through pipe and display
        printf("Series of first %d Fibonacci numbers\n",n);
        int fibs[n];
        for(int i = 0; i < n; i++) {
            read(fd[0], &fnum, sizeof(fnum));
            fibs[i] = fnum;
            printf("%d",fibs[i]);
            if(i == n-1) printf("\n");
            else printf(", ");
        }

        // Display all prime numbers from the fibonacci series
        printf("Prime Fibonacci numbers are: \n");
        for(int i = 0; i < n; i++) {
           if(isPrime(fibs[i])) printf("%d at position %d\n",fibs[i],i+1);
        }

        close(fd[0]);
    } else {
        // Child process

        // Array of fibonacci series
        int fib[n];
        fib[0] = 0;
        fib[1] = 1;
        for(int i = 2; i < n; i++)
            fib[i] = fib[i-1]+fib[i-2];
        
        // Send all fibonacci numbers to parent process through pipe
        for(int i = 0; i < n; i++) {
            write(fd[1], &fib[i], sizeof(fib[i]));
        }

        close(fd[1]);
    }
}