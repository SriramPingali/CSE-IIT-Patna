/*
    CS359 Assignment 7
    
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Cyclic Redundancy Check - TCP client program in C

    A string is taken as input from user by the client program. 
    The string is sent to the server in the form of ASCII and CRC remainders. 
    The ASCII code of each character is taken, split into parts containing 
    4-bits each, then the corresponding CRC remainders are calculated using 
    the CRC key = 1011, and those 7-bits are sent as a single packet to the 
    server. The server uses the data and remainder bits to check if the data 
    is corrupted. If the data got corrupted, it requests the client to send 
    those bits once again. Using this, the corresponding characters are 
    continuously concatenated to reconstruct the string being sent by the 
    client.

    Finally, the server acknowledges the client with md5sum of received string. 
    If the md5sums at client and server sides match, the transmission was 
    successful and Cyclic Redundancy Check's purpose is served.

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
    for(int i = 0; i < MAXBITS/2; i++) {
        int temp = binary[i];
        binary[i] = binary[MAXBITS-1-i];
        binary[MAXBITS-1-i] = temp;
    }
}

// function to check whether dividend is zero
int isDividendZero(int pkt[]) {
    for(int i = 0; i < 4; i++) {
        if(pkt[i] == 1) return 0;
    }
    return 1;
}

// function to create packet containing data bits and CRC remainder bits
void createPacket(int binary[], int start, int pkt[]) {
    
    // key for crc
    int key[4] = {1,0,1,1};

    // create a copy of data to be sent
    for(int i = 0; i < 4; i++) {
        pkt[i] = binary[start+i];
    }

    // append the data with zeroes
    for(int i = 4; i < 7; i++) {
        pkt[i] = 0;
    }

    printf("Data to be sent = ");
    for(int it = 0; it < 4; it++) printf("%d",pkt[it]);
    printf("\n");
    
    // CRC Generator 
    int i = 0;
    while(pkt[i] == 0) i++;
    while(!isDividendZero(pkt)) {
        for(int j = 0; j < 4; j++) {
            pkt[i+j] ^= key[j];
        }

        int p = 0;
        while(pkt[p] == 0) p++;
        i = p;
    }

    for(int i = 0; i < 4; i++) {
        pkt[i] = binary[start+i];
    }

    printf("Data with remainder = ");
    for(int it = 0; it < 4; it++) printf("%d",pkt[it]);
    printf(" ");
    for(int it = 4; it < 7; it++) printf("%d",pkt[it]);
    printf("\n");
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
        // ascii of character
        int number = input[i];
        
        if(input[i] != '\0') {
            printf("Sending Character: \"%c\" (ASCII %d)\n",input[i],number);
        } else {
            printf("Sending Character: \"\\0\" (ASCII %d)\n",number);
        }

        // binary form of ascii of character
        int binary[MAXBITS] = {0};
        decToBin(number,binary);

        // create packets for character's ascii value
        for(int i = 0; i < MAXBITS; i += 4) {
            int pkt[7];
            createPacket(binary,i,pkt);
            send(sockfd, pkt, sizeof(pkt), 0);
            
            char stat[10] = {0};
            recv(sockfd,stat,sizeof(stat),0);
            
            if(strcmp(stat,"ERROR") == 0) {
                i -= 4;
                continue;
            }

            usleep(1);
        }

        // end of string, stop transmission
        if(input[i] == '\0') break;
        printf("-------------\n");
    }

    printf("[+]User message sent successfully.\n");

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