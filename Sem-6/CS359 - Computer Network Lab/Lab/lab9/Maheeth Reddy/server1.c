/*
    CS359 Computer Networks Lab
            Assignment 9
    
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

// function to check whether a character is vowel or not
int isVowel(char ch) {
    switch (ch) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
            return 1;
        
        default:
            return 0;
    }
}

// function to get count of vowels in a string
int countVowel(char str[]) {
    int count = 0, len = strlen(str);
    for(int i = 0; i < len; i++) {
        count += isVowel(str[i]);
    }
    return count;
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
    printf("Received \"%s\" from client\n",buffer);

    // count vowels in received string
    int len = strlen(buffer);
    printf("It has %d vowel(s):\n",countVowel(buffer));
    for(int i = 0; i < len; i++) {
        if(isVowel(buffer[i])) {
            printf("( %c ) at position %d\n",buffer[i],i);
        }
    }
    printf("\n");

    printf("[+]Closing the connection.\n");
    return 0;
}