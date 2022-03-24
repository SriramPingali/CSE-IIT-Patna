/*
    CS359 Assignment 3
    
    Name: M Maheeth Reddy
    Roll: 1801CS31

    TCP client C code
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024
#define HELO "HELO"

char filename[SIZE] = {0};

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

void send_file(FILE *fp, int sockfd){
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
        perror("[-]Error in sending file.");
        exit(1);
        }
        bzero(data, SIZE);
    }
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char buffer[SIZE];
    int filesize = 0;

    printf("Enter filename: ");
    scanf("%s",filename);

    filesize=findSize(filename);

    if(filesize == -1) {
        printf("File Not Found!\n");
        exit(EXIT_FAILURE);
    }

    char md5sum[SIZE] = {0};
    computeMD5(fp,md5sum);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(e == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Server.\n");

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("[-]Error in reading file.");
        exit(1);
    }

    char hello[2*SIZE] = HELO;
    strcat(hello," ");
    strcat(hello,filename);
    strcat(hello," ");
    sprintf(hello+strlen(hello),"%d",filesize);
    hello[strlen(hello)] = '\0';
    printf("Hello:\t%s\n",hello);

    send(sockfd, hello, sizeof(hello), 0);
    printf("C: %s\n",hello);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("S: %s\n",buffer);

    printf("[+]Sending File data.\n");
    send_file(fp, sockfd);
    printf("[+]File data sent successfully.\n");

    char md5rec[SIZE] = {0};
    recv(sockfd,md5rec,SIZE,0);

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
}