/*
    CS359 Assignment 3
    
    Name: M Maheeth Reddy
    Roll: 1801CS31

    UDP client C code
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
#define HELO "hello"

char filename[SIZE] = {0};

typedef struct udp_segment {
  uint seqnum;
  uint length;
  char data[SIZE+1];
} udp_segment;

void computeMD5(FILE *fp, char *md5){
  char cmd[SIZE] = {0};
  strcpy(cmd,"md5sum ");
  strcat(cmd,filename);
  fp = popen(cmd, "r");
	fscanf(fp,"%s", md5);
	pclose(fp);
}

// function to check size of a file
long int findSize(char* filename) {
  FILE* fp = fopen(filename, "r");
  
  if (fp == NULL) {
    return -1;
  }

  fseek(fp,0L,SEEK_END);
  // calculating the size of the file
  long int res = ftell(fp);
  fclose(fp);
  return res;
}

// Driver code
int main() { 
  int sockfd;
  char buffer[SIZE];
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
  
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

  int n, len;
  
  printf("Enter filename: ");
  scanf("%s",filename);

  int filesize = findSize(filename);
  if(filesize == -1) {
    printf("File Not Found!\n");
    exit(EXIT_FAILURE);
  }

  char md5sum[SIZE] = {0};

  // sending hello message
  char hello[2*SIZE] = HELO;
  strcat(hello," ");
  strcat(hello,filename);
  strcat(hello," ");
  sprintf(hello+strlen(hello),"%d ",filesize);
  sprintf(hello+strlen(hello),"%d",(filesize/1024)+(filesize%1024));
  hello[strlen(hello)] = '\0';
  printf("Hello:\t%s\n",hello);
  sendto(sockfd, hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
  printf("C: %s\n",hello);
  
  // receive hello from server
  n = recvfrom(sockfd, (char *)buffer, SIZE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
  buffer[n] = '\0';
  printf("S: %s\n", buffer);
  
  FILE* fp = fopen(filename,"r");
  computeMD5(fp,md5sum);
  int segnum=0;
  while(1) {
    char data[SIZE+1] = {0};
    int count = fread(data, sizeof(char), SIZE, fp);
    if( !count ) {
      if(ferror(fp)) {
        perror("Error reading file");
      } else if (feof(fp)) {
        perror("EOF reached\n");
      }

      udp_segment finish;
      finish.seqnum=segnum;
      finish.length=sizeof("finish")+8;
      strcpy(finish.data,"finish\0");
      sendto(sockfd, &finish, sizeof(finish), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
      break;
    }

    udp_segment seg;
    seg.seqnum=segnum;
    seg.length=count*sizeof(char)+8;
    printf("strlen(data)=%ld sizeof(data)=%ld\n",strlen(data),sizeof(data));
    data[strlen(data)] = 0;

    strcpy(seg.data,data);

    printf("Sending segment...\n");
    printf("SeqNum:\t%d\n", seg.seqnum);
    printf("Length:\t%d\n", seg.length);
    printf("Data:\t%s\n",seg.data);

    int ackReceived = 0;
    udp_segment *ack = (udp_segment*)malloc(sizeof(udp_segment));
    char null[SIZE+1] = {0};
    ack->seqnum=0;
    ack->length=0;
    strcpy(ack->data,null);
    while (ackReceived == 0) {
        if (sendto(sockfd, &seg, sizeof(seg), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
            perror("Error in re-sending segment.");
            exit(1);
        } else {
          int ackrec = recvfrom(sockfd, ack, sizeof(*ack), MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
          if(ackrec != -1 && strcmp(ack->data,"done")==0) {
            printf("Received acknowledgement %d\n",seg.seqnum);
            ackReceived = 1;
            ack->seqnum=0;
            ack->length=0;
            strcpy(ack->data,null);
            free(ack);
            break;
          }
        }
    }
    
    segnum++;
  }

  char md5rec[SIZE] = {0};
  int x = recvfrom(sockfd, md5rec, sizeof(md5rec), 0, (struct sockaddr *) &servaddr, &len);

  // Compare MD5 checksums
  if(strcmp(md5sum,md5rec)==0) {
    printf("[+]MD5 Matched\n");
  } else {
    printf("[-]MD5 Not Matched\n");
  }
  
  printf("\tmd5sum of %s:\t%s\n",filename,md5sum);
  printf("\tmd5sum received:\t%s\n",md5rec);

  printf("[+]Closing the connection.\n");
  close(sockfd);

  return 0;

  close(sockfd); 
  return 0; 
}