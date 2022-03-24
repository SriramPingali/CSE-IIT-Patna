/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Midsem Assignent
                Question 2a
    
    Fork System Call
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// function to calculate n-th term of series
double term(int n) {
    double mod = 1.0/n;
    return (n%2) ? mod : -mod;
}

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

// driver code
int main(int argc, char **argv) {
    // input validation
    if(argc < 2) {
        printf("Error: no value of \"n\" is given\n");
        return 1;
    }

    // n must be greater than or equal to 1
    int n = atoi(argv[1]);
    printf("n = %d\n",n);
    if(n < 1) {
        printf("Error: \"n\" must be atleast 1\n");
        return 1;
    }

    // variables to store various sums
    double sumNeg = 0, sumPosPrime = 0, sumPosNonPrime = 0, final = 0;

    // file descriptors to send values from child processes to parent process
    int fd1[2],fd2[2],fd3[2];
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);

    pid_t pid1,pid2,pid3;
    pid1 = fork();
    if(pid1 == 0) {
        // child process 1
        close(fd1[0]);

        for(int i = 2; i <= n; i+=2) {
            // if i is even, i-th term is negative
            sumNeg += term(i);
        }
        printf("Sum of negative numbers in the series is %lf\n",sumNeg);

        // send sum to parent process
        write(fd1[1], &sumNeg, sizeof(sumNeg));
        close(fd1[1]);
    } else {
        pid2 = fork();
        if(pid2 == 0) {
            // child process 2
            close(fd2[0]);

            for(int i = 1; i <= n; i+=2) {
                // if i is odd, i-th term is positive, check whether i is prime
                if(isPrime(i)) {
                    sumPosPrime += term(i);
                }
            }
            printf("Sum of positive prime numbers in the series is %lf\n",sumPosPrime);

            // send sum to parent process
            write(fd2[1], &sumPosPrime, sizeof(sumPosPrime));
            close(fd2[1]);
        } else {
            pid3 = fork();
            if(pid3 == 0) {
                // child process 3
                close(fd3[0]);

                for(int i = 1; i <= n; i+=2) {
                    // if i is odd, i-th term is positive, check whether i is not prime
                    if(!isPrime(i)) {
                        sumPosNonPrime += term(i);
                    }
                }
                printf("Sum of positive non-prime numbers in the series is %lf\n",sumPosNonPrime);

                // send sum to parent process
                write(fd3[1], &sumPosNonPrime, sizeof(sumPosNonPrime));
                close(fd3[1]);
            } else {
                // parent process

                // receive sum from child 1
                close(fd1[1]);
                read(fd1[0], &sumNeg, sizeof(sumNeg));
                close(fd1[0]);

                // receive sum from child 2
                close(fd2[1]);
                read(fd2[0], &sumPosPrime, sizeof(sumPosPrime));
                close(fd2[0]);

                // receive sum from child 3
                close(fd3[1]);
                read(fd3[0], &sumPosNonPrime, sizeof(sumPosNonPrime));
                close(fd3[0]);

                // final result is sum of sums return bythe 3 child processes
                // print final result of series
                final = sumNeg + sumPosPrime + sumPosNonPrime;
                printf("Final result of series is %lf\n",final);
            }
        }
    }

    return 0;
}