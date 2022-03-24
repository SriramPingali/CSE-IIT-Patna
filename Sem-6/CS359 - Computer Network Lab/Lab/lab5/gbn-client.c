/*
    CS359 Assignment 5

    Name: M Maheeth Reddy
    Roll: 1801CS31

    Go-Back-N client code
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024
#define WINDOW 5
#define IPFILE "gistfile.txt"

typedef struct Packet {
    uint sno;
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

long int fileSize(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) return -1;

    fseek(fp,0L,SEEK_END);
    // calculating the size of the file
    long int res = ftell(fp);
    fclose(fp);
    return res;
}

// Driver code
int main() {
    int sockfd;
    Packet hello = createPacket(794,"Hello from client");
    Packet* buffer = (Packet *)malloc(sizeof(Packet));
    
    struct sockaddr_in servaddr;
    
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Send a hello message to server
    sendPacket(sockfd,&hello,&servaddr);
    printf("C: %s\n",hello.data);
    
    // receive a hello message from server
    int len;
    int n = recvfrom(sockfd, (Packet *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buffer->data[strlen(buffer->data)] = '\0';
    printf("Server : %s\n", buffer->data);
    
    // Open a file of size > 1MB, send it in packets of size 1kB each.
    FILE* fp = fopen(IPFILE,"r");
    int filesize = fileSize(IPFILE);
    
    // Read the input file and convert it to packets
    int numPacks = filesize/1024 + 2;
    Packet pkt[numPacks];
    memset(&pkt, 0, sizeof(pkt));
    
    printf("Creating Packets...\n");
    int i = 0;
    while(1) {
        char data[MAXLINE+1] = {0};
        int count = fread(data, sizeof(char), MAXLINE, fp);

        // end condition
        if( count == 0 ) {
            if(ferror(fp)) {
                perror("Error reading file");
            } else if (feof(fp)) {
                perror("EOF reached");
            }

            pkt[i].sno = i;
            pkt[i].isFinal = 1;
            strcpy(pkt[i].data,"file transfer complete");
            i++;
            
            break;
        }

        pkt[i].sno = i;
        data[strlen(data)] = '\0';
        strcpy(pkt[i].data,data);
        pkt[i].isFinal = 0;

        i++;
    }
    fclose(fp);

    // Send packets
    int base = 0;
    while(1) {
        int i;
        for(i = base; i < base+WINDOW; i++) {
            sendPacket(sockfd, &pkt[i], &servaddr);
            printf("Sending packet %d...\n",pkt[i].sno);

            int n = recvfrom(sockfd, (Packet *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            buffer->data[strlen(buffer->data)] = '\0';
            int ackno = buffer->sno;

            // If acknowledgement is received properly, p
            if (n != -1 && ackno == i) {
                printf("Acknowledgement %d received\n", buffer->sno);
                if(pkt[i].isFinal == 1) break;
                base++;
            } else if (n != -1 && ackno < i) {
                i = ackno;
            } else {
                i--;
            }
        }

        if(pkt[i].isFinal == 1) break;
    }
    
    close(sockfd);
    return 0;
}