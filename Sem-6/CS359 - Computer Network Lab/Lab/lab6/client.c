/*
    CS359 Assignment 6
    
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Hamming Code - TCP client C code

    A string is taken as input from user by the client program. The string is sent
    to the server in the form of ASCII and Hamming Codes. The ASCII code of each
    character is taken, split into parts containing 4-bits each, then the parity
    bits are calculated and the 7-bits are sent as a single packet to the server.
    The server uses the parity bits to check if the data is corrupted. Necessary 
    corrections are made and the ASCII value is re-calculated. Using this, the 
    corresponding characters are continuously concatenated to reconstruct the
    string being sent by the client.

    Finally, the server acknowledges the client with md5sum of received string.
    If the md5sums at client and server sides match, the transmission was successful
    and Hamming Code's purpose is served.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1048576
#define MAXBITS 32

// function to compute md5sum
void computeMD5(FILE *fp, char *str, char *md5){
    char cmd[SIZE] = {0};
    
    strcpy(cmd,"echo -n \"");
    strcat(cmd,str);
    strcat(cmd,"\" | md5sum");
    
    fp = popen(cmd, "r");
    fscanf(fp,"%s", md5);
    pclose(fp);
}

// function to convert decimal to binary
void decToBin(int num, int binary[]) {
    int i = 0;
    while(num > 0) {
        binary[i] = num%2;
        num/=2;
        i++;
    }
}

// function to create packet containing data bits and parity bits
void createPacket(int binary[], int start, int pkt[]) {
    // copy bits
    pkt[2] = binary[start+0];
    pkt[4] = binary[start+1];
    pkt[5] = binary[start+2];
    pkt[6] = binary[start+3];

    // calculate parity
    pkt[0] = pkt[2]^pkt[4]^pkt[6];
    pkt[1] = pkt[2]^pkt[5]^pkt[6];
    pkt[3] = pkt[4]^pkt[5]^pkt[6];
}

int main() {
    // Take input from user
    char input[SIZE] = {0};
    printf("Enter a message: ");
    scanf("%[^\n]c",input);

    // calculate md5sum of the input
    FILE* fp;
    char md5sum[SIZE] = {0};
    computeMD5(fp,input,md5sum);

    // setup client side socket
    char *ip = "127.0.0.1";
    int port = 8080;

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

    printf("[+]Sending Packets.\n\n");

    // send the input string
    for(int i = 0; i < SIZE; i++) {
        int number = input[i];
        int binary[MAXBITS] = {0};
        decToBin(number,binary);

        for(int i = 0; i < MAXBITS; i += 4) {
            int pkt[7];
            createPacket(binary,i,pkt);
            send(sockfd, pkt, sizeof(pkt), 0);
            usleep(1);
        }

        if(input[i] == '\0') break;
    }

    printf("[+]File data sent successfully.\n");

    // get md5sum from server
    char md5rec[SIZE] = {0};
    recv(sockfd,md5rec,sizeof(md5rec),0);

    // compare md5sums at client,server
    printf("MD5 client:\t%s\n",md5sum);
    printf("MD5 server:\t%s\n",md5rec);
    if(strcmp(md5sum,md5rec) == 0) {
        printf("[+]MD5 Matched\n");
    } else {
        printf("[-]MD5 Not Matched\n");
    }
    
    printf("[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}