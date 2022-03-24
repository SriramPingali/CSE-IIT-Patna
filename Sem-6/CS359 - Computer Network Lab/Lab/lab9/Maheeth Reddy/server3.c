/*
    CS359 Computer Networks Lab
            Assignment 9
    
    Name: M. Maheeth Reddy
    Roll: 1801CS31

    Question 3

    >>>> Server Program <<<<
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

// function to print array of integers
void printArray(int array[], int n) {
    printf("[");
    for(int i = 0; i < n; i++) {
        printf("%d",array[i]);
        if(i != n-1) printf(", ");
        else printf("]");
    }
    printf("\n");
}

// function to calculate sum of integers in an array
int sumOfArray(int array[], int n) {
    int sum = 0;
    for(int i = 0; i < n; i++) {
        sum += array[i];
    }
    return sum;
}

int main(){
    char *ip = "127.0.0.1";
    int port = 8080;
    
    // make connection with client
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    struct sockaddr_in server_addr, new_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    int e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e < 0) {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successfull.\n");

    if (listen(sockfd, 10) == 0) {
        printf("[+]Listening....\n");
    } else {
        perror("[-]Error in listening");
        exit(1);
    }
    printf("\n");
    
    socklen_t addr_size = sizeof(new_addr);
    int new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

    // receive number of integers client is going to send
    int num = 0;
    recv(new_sock, &num, sizeof(num), 0);
    printf("Received number of integers (%d) from client\n",num);

    // receive array of integers from client
    int buffer[num];
    recv(new_sock, buffer, sizeof(buffer), 0);
    printf("Received array of integers from client:\n");
    printArray(buffer,num);
    
    // send sum of array of integers to client
    int sum = sumOfArray(buffer,num);
    send(new_sock, &sum, sizeof(sum), 0);
    printf("\nSending sum of the integers (%d) to client\n\n",sum);

    printf("[+]Closing the connection.\n");
    return 0;
}