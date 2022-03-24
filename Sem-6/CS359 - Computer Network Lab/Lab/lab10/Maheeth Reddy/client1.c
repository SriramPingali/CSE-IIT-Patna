/*
    CS359 Computer Networks Lab
            Assignment 10
    
    Name: M. Maheeth Reddy
    Roll: 1801CS31

    Question 1

    >>>> Client Program <<<<
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;

    // message to send
    char message[SIZE] = {0};
    printf("Enter a message: ");
    scanf("%[^\n]s",message);

    // creating connection with server
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

    // sending message
    send(sockfd, message, SIZE, 0);
    printf("\nClient sent \"%s\" to server\n\n",message);

    // receiving uppercase message
    char uppercase[SIZE] = {0};
    recv(sockfd, uppercase, SIZE, 0);
    printf("Received message in uppercase from server:\n");
    printf("\t%s\n",uppercase);

    printf("\n[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}