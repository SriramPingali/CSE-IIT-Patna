/*
    CS359 Computer Networks Lab
            Assignment 10
    
    Name: M. Maheeth Reddy
    Roll: 1801CS31

    Question 3

    >>>> Client Program <<<<
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;

    // take number of integers as input from user
    int num;
    printf("Enter number of integers: ");
    scanf("%d",&num);
    
    // take array of integers as input from user
    int array[num];
    printf("Enter integers: ");
    for(int i = 0; i < num; i++) {
        scanf("%d",&array[i]);
    }
    printf("\n");

    // create connection with server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    int e = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(e == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Server.\n");

    // send number of integers to server
    send(sockfd, &num, sizeof(num), 0);
    printf("\nClient sent number of integers (%d) to server\n",num);
    
    // send array of integers to server
    send(sockfd, array, sizeof(array), 0);
    printf("Client sent the array of integers to server\n");
    
    printf("\n[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}