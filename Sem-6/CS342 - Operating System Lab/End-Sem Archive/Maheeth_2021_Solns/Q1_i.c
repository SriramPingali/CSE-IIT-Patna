/*
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Operating Systems Lab - Endsem
        Question 1 Part (i)
*/
#include <stdio.h>

// the code is self explanatory
// the print statements are sufficient to understand the code
// explanation behind the approach is written in README
int main() {
    int c,r;
    printf("No. of copies of resource (r): ");
    scanf("%d",&r);
    printf("No. of copies of resource required by each process (c): ");
    scanf("%d",&c);

    if(c == 1) {
        printf("There can be unlimited no. of processes\n");
        printf("Hence, n = infinity\n");
    } else if(c > 1) {
        printf("Maximum of %d process(es) are required to ensure a deadlock free operation\n",(r-1)/(c-1));
    } else {
        printf("Invalid Input\n");
    }
}