/*
    CS359 Assignment 7
    
    Name: M Maheeth Reddy
    Roll: 1801CS31

    Cyclic Redundancy Check - TCP server program in C

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

// function to check whether remainder is zero
int isRemZero(int pkt[]) {
    for(int i = 4; i < 7; i++) {
        if(pkt[i] == 1) return 0;
    }
    return 1;
}

// function for CRC error detection
int crc_detect(int pkt[]) {
    // key for crc
    int key[4] = {1,0,1,1};

    // create a copy of data received
    int copy[4];
    for(int i = 0; i < 4; i++) {
        copy[i] = pkt[i];
    }

    printf("Data and remainder received: ");
    for(int it = 0; it < 4; it++) printf("%d",pkt[it]);
    printf(" ");
    for(int it = 4; it < 7; it++) printf("%d",pkt[it]);
    printf("\n");

    // perform CRC checker
    int i = 0;
    while(i <= 3) {
        if(pkt[i]) {
            for(int j = 0; j < 4; j++) {
                pkt[i+j] ^= key[j];
            }
        }
        i++;
    }

    // if remainder is not zero, then data is corrupted
    if(!isRemZero(pkt)) {
        printf("ERROR during transmission");
        return 1;
    }

    for(int i = 0; i < 4; i++) {
        pkt[i] = copy[i];
    }

    printf("Data Received: ");
    for(int it = 0; it < 4; it++) printf("%d",pkt[it]);
    printf("\t");

    printf("Remainder after CRC checking: ");
    for(int it = 4; it < 7; it++) printf("%d",pkt[it]);
    printf("\n");
    
    printf("Data was received properly\n\n");

    return 0;
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

        // get the 7-bits and apply crc error detection
        for(int i = 0; i < MAXBITS; i+=4) {
            int pkt[7];
            recv(new_sock, pkt, sizeof(pkt), 0);
            
            if(crc_detect(pkt) == 1) {
                send(new_sock,"ERROR",sizeof("ERROR"),0);
                i-=4;
                continue;
            } else {
                binary[i+0] = pkt[0];
                binary[i+1] = pkt[1];
                binary[i+2] = pkt[2];
                binary[i+3] = pkt[3];
                send(new_sock,"SUCCESS",sizeof("SUCCESS"),0);
            }
        }

        // get ascii of data received
        int recNumber = 0;
        for(int i = 0; i < MAXBITS; i++) {
            if(binary[i]) {
                recNumber += (int)pow2(MAXBITS-1-i);
            }
        }

        // write corresponding character into receiving string
        if(recNumber != 0) printf("Character \"%c\" (ASCII %d) was received from client\n",recNumber,recNumber);
        else printf("Character \"\\0\" (ASCII %d) was received from client\n",recNumber);
        printf("----------------------------------------\n");
        
        char temp[2] = {recNumber,'\0'};
        strcat(recvmsg,temp);
        if(recNumber == '\0') break;
    }
    printf("[+]Data received successfully.\n");

    printf("Message sent by client:\n\t\"%s\"\n",recvmsg);

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