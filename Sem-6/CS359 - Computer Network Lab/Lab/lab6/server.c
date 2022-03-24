/*
    CS359 Assignment 6
    
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Hamming Code - TCP server C code

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

// function to compute exponents of 2
int pow2(int n) {
    int r = 1;
    for(int i = 0; i < n; i++) r *= 2;
    return r;
}

// function for hamming code error detection and correction
void hamming(int pkt[]) {
    int a = pkt[3]^pkt[6]^pkt[5]^pkt[4];
    int b = pkt[1]^pkt[6]^pkt[5]^pkt[2];
    int c = pkt[0]^pkt[6]^pkt[4]^pkt[2];

    int ind = c + 2*b + 4*a;
    if(ind != 0) {
        pkt[ind-1] = !pkt[ind-1];
    }
}

int main() {
    // setup server side socket
    char *ip = "127.0.0.1";
    int port = 8080;

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
    printf("[+]Binding successful.\n");

    if (listen(sockfd, 10) == 0) {
        printf("[+]Listening....\n");
    } else {
        perror("[-]Error in listening");
        exit(1);
    }
    
    socklen_t addr_size = sizeof(new_addr);
    int new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

    // stores the received string
    char recvmsg[SIZE] = "";

    // receive data from client
    for(int i = 0; i < SIZE; i++) {
        int binary[MAXBITS] = {0};

        // get the 7-bits and apply hamming error correction
        for(int i = 0; i < MAXBITS; i+=4) {
            int pkt[7];
            recv(new_sock, pkt, sizeof(pkt), 0);
            hamming(pkt);

            binary[i+0] = pkt[2];
            binary[i+1] = pkt[4];
            binary[i+2] = pkt[5];
            binary[i+3] = pkt[6];
        }

        // get ascii of data received
        int recNumber = 0;
        for(int i = 0; i < MAXBITS; i++) {
            if(binary[i]) {
                recNumber += (int)pow2(i);
            }
        }

        // write corresponding character into receiving string
        if(recNumber != '\0') {
            printf("Number %d ( %c ) was received from client\n",recNumber,recNumber);
        }
        char temp[2] = {recNumber,'\0'};
        strcat(recvmsg,temp);
        if(recNumber == '\0') break;
    }
    printf("------------------\n");
    printf("[+]Data received successfully.\n");

    printf("\"%s\" was received from client\n",recvmsg);

    // compute md5sum of received string
    FILE* fp;
    char md5sum[SIZE] = {0};
    computeMD5(fp,recvmsg,md5sum);
    printf("MD5:\t%s\n",md5sum);

    // send md5 to client
    if(send(new_sock,md5sum,sizeof(md5sum),0) != -1) {
        printf("[+]MD5 sent\n");
    } else {
        printf("Couldn\'t send MD5\n");
    }

    printf("[+]Closing the connection.\n");
    return 0;
}