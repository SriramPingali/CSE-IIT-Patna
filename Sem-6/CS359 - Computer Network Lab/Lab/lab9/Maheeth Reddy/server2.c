/*
    CS359 Computer Networks Lab
            Assignment 9
    
    Name: M. Maheeth Reddy
    Roll: 1801CS31

    Question 2

    >>>> Server Program <<<<
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

// function to convert uppercase character to lowercase
char lower(char ch) {
    if(ch >= 'A' && ch <= 'Z') return ch+'a'-'A';
    return ch;
}

// function to check if string is palindrome
int isPalindrome(char str[]) {
    int len = strlen(str);

    for(int i = 0; i < len/2; i++) {
        if(lower(str[i]) != lower(str[len-1-i])) {
            return 0;
        }
    }

    return 1;
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

    // receive string from client
    char buffer[SIZE] = {0};
    recv(new_sock, buffer, SIZE, 0);
    printf("Received \"%s\" from client\n",buffer);

    // check if received string is a palindrome
    if(isPalindrome(buffer)) {
        printf("It is a palindrome string\n");
    } else {
        printf("It is not a palindrome string\n");
    }

    printf("\n[+]Closing the connection.\n");
    return 0;
}