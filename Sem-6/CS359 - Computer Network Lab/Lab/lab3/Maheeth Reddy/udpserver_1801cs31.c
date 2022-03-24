/*
    CS359 Assignment 3
    
    Name: M Maheeth Reddy
    Roll: 1801CS31

    UDP server C code
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
#define SIZE 1024
#define RECV_FILE "recv.txt"

typedef struct udp_segment {
    uint seqnum;
    uint length;
    char data[SIZE+1];
} udp_segment;

void computeMD5(FILE *fp, char *md5){
	fp = popen("md5sum recv.txt", "r");
	fscanf(fp,"%s", md5);
	pclose(fp);
}

int main() { 
    int sockfd; 
    char buffer[SIZE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n;
    len = sizeof(cliaddr);
    n = recvfrom(sockfd, (char *)buffer, SIZE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buffer[n] = '\0';

    printf("C: %s\n", buffer);
    sendto(sockfd, "hello", strlen("hello"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    printf("S: hello\n");

    FILE* fp = fopen("recv.txt","w");
    udp_segment* seg = (udp_segment*)malloc(sizeof(udp_segment));
    
    while(1) {
        char null[SIZE+1] = {0};
        seg->seqnum=0;
        seg->length=0;
        strcpy(seg->data,null);
        int n = recvfrom(sockfd, seg, sizeof(*seg), MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);

        if(strcmp(seg->data,"finish") == 0) {
            fclose(fp);
            break;
        }

        printf("SeqNum:\t%d\n", seg->seqnum);
        printf("Length:\t%d\n", seg->length);
        printf("Data:\t%s\n",seg->data);
        if (n != -1) {
            fprintf(fp,"%s",seg->data);

            // acknowledgement
            printf("Sending acknowledgement %d...\n",seg->seqnum);
            udp_segment ack;
            ack.seqnum = seg->seqnum;
            strcpy(ack.data,"done");
            ack.length = 4+4+sizeof(ack.data);
            if(sendto(sockfd, &ack, sizeof(ack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len) != -1) {
                printf("Acknowledgement %d sent\n",seg->seqnum);
            }
        }
    }

    // calculate MD5 checksum and send to client
    char md5sum[SIZE] = {0};
    fp = fopen("recv.txt","r");
    computeMD5(fp,md5sum);
    printf("md5sum:\t%s\n",md5sum);
    
    if(sendto(sockfd, md5sum, strlen(md5sum), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len) == -1) {
        perror("[-]Error in sending md5 checksum");
    } else {
        printf("[+]MD5 checksum of received file is sent to client.\n");
    }

    printf("[+]Closing the connection.\n");

    return 0; 
} 