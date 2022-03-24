/*
    CS359 Assignment 3
    
    Name: M Maheeth Reddy
    Roll: 1801CS31

    TCP server C code
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024
#define HELO "HELO"
#define RECV_FILE "recv.txt"
#define END "END"

void computeMD5(FILE *fp, char *md5){
	fp = popen("md5sum recv.txt", "r");
	fscanf(fp,"%s", md5);
	pclose(fp);
}

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = RECV_FILE;
  char buffer[SIZE];

  fp = fopen(filename, "w");

  if (fp == NULL) {
    printf("can\'t open new file\n");
    return;
  } else {
    printf("File opened successfully\n");
  }

  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);

    if (n <= 0){
      fclose(fp);
      return;
    }
    
    fprintf(fp,"%s",buffer);

    if(strcmp(buffer,"END")==0) {
      fclose(fp);
      return;
    }

    bzero(buffer,SIZE);
  }

  fclose(fp);
  return;
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE] = {0};

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
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

  printf("-------HERE-------\n");
  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

  recv(new_sock, buffer, SIZE, 0);
  printf("C: %s\n",buffer);
  send(new_sock, HELO, sizeof(HELO), 0);
  printf("S: %s\n",HELO);

  write_file(new_sock);
  printf("[+]Data written in the file successfully.\n");

  char md5sum[SIZE] = {0};
  FILE* fp;
  computeMD5(fp,md5sum);
  printf("md5sum:\t%s\n",md5sum);
  
  if(send(new_sock, md5sum, sizeof(md5sum), 0) == -1) {
    perror("[-]Error in sending md5 checksum");
  } else {
    printf("[+]MD5 checksum of received file is sent to client.\n");
  }

  printf("[+]Closing the connection.\n");
  return 0;
}