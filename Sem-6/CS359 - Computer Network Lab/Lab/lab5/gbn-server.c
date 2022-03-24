/*
    CS359 Assignment 5

    Name: M Maheeth Reddy
    Roll: 1801CS31

    Go-Back-N server code
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

typedef struct Packet {
    int sno;
    char data[MAXLINE+1];
    int isFinal;
} Packet;

Packet createPacket(int sno, char data[]) {
    Packet temp;
    memset(&temp, 0, sizeof(temp));
    temp.sno = sno;
    strcpy(temp.data,data);
    temp.isFinal = 0;
    return temp;
}

int sendPacket(int sockfd, Packet* data, struct sockaddr_in* addr) {
    return sendto(sockfd, data, sizeof(*data), MSG_CONFIRM, (const struct sockaddr *) addr, sizeof(*addr));
}

int main() {
    int sockfd;
    Packet hello = (Packet){800,"Hello from server"};
    Packet* buffer = (Packet *)malloc(sizeof(Packet));
    struct sockaddr_in servaddr, cliaddr;
    
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // hello from client
    int len = sizeof(cliaddr);
    int n = recvfrom(sockfd, (Packet *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buffer->data[strlen(buffer->data)] = '\0';
    printf("Client : %s\n", buffer->data);

    // hello to server
    sendPacket(sockfd, &hello, &cliaddr);
    printf("Hello message sent.\n");
    
    // receiving file
    FILE* fp = fopen("recv.txt","w");
    
    // collect pockets in this variable
    Packet* seg = (Packet*)malloc(sizeof(Packet));
    // current segment number and previous segment number
    int cur = 0, prev = 0;
    
    while(1) {
        memset(seg, 0, sizeof(*seg));
        int n = recvfrom(sockfd, seg, sizeof(*seg), MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);

        printf("Receiving Packet %d...\n", seg->sno);

        Packet ack;
        if(n != -1) {
            printf("Data:\t%s\n",seg->data);

            cur = seg->sno;
            fprintf(fp,"%s",seg->data);
            
            // acknowledgement
            printf("Sending acknowledgement %d...\n",cur);
            ack.sno = cur;
            strcpy(ack.data,"done");
            
            if(sendPacket(sockfd,&ack,&cliaddr) != -1) {
                printf("Acknowledgement %d sent\n",seg->sno);
            }

             if(seg->isFinal == 1) {
                fclose(fp);
                break;
            }

            prev = cur;
        } else {
            if(seg->isFinal == 1) {
                fclose(fp);
                break;
            }
            
            // acknowledgement
            printf("Sending acknowledgement %d...\n",cur);
            Packet ack;
            ack.sno = cur;
            strcpy(ack.data,"done");
            
            if(sendPacket(sockfd,&ack,&cliaddr) != -1) {
                printf("Acknowledgement %d sent\n",seg->sno);
            }
        }
    }

    close(sockfd);
    return 0;
}