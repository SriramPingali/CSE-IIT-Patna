#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    // FORK CALLS TO GET DESIRED OUTPUT
    fork() && fork() || fork();

    // Corresponding fork tree:
    //   P -> C1 -> C2
    //   |    |
    //   P    C1
    //  / \
    // P  C3 -> C4
    //    |
    //    C3
    
    printf("Hello");

    return 0; 
}