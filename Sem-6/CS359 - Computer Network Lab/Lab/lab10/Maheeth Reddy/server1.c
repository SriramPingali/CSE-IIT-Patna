/*
    CS359 Computer Networks Lab
            Assignment 10
    
    Name: M. Maheeth Reddy
    Roll: 1801CS31

    Question 1

    >>>> Server Program <<<<
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

// function to convert string to lowercase
void toLowercase(char str[]) {
    int len = strlen(str);
    for(int i = 0; i < len; i++) {
        if(str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i]-'A'+'a';
        }
    }
}

// function to convert string to uppercase
void toUppercase(char str[]) {
    int len = strlen(str);
    for(int i = 0; i < len; i++) {
        if(str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i]-'a'+'A';
        }
    }
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

    // receive string into buffer
    char buffer[SIZE] = {0};
    recv(new_sock, buffer, SIZE, 0);
    printf("Received the message \"%s\" from client\n",buffer);

    // convert buffer to lowercase
    toLowercase(buffer);
    printf("Message in lowercase: %s\n",buffer);
    
    // convert buffer to uppercase
    toUppercase(buffer);
    printf("Sending Message in Uppercase to client\n");
    send(new_sock,buffer,SIZE,0);

    printf("\n[+]Closing the connection.\n");
    return 0;
}