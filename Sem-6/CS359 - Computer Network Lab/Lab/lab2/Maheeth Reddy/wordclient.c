/*
    CS359 Computer Networks Lab
            Assignment 2
    
    Name: M. Maheeth Reddy
    Roll: 1801CS31

    >>>> Client Program <<<<
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT    8080
#define MAXLINE 1024

int main() { 
    int sockfd;
    char *hello = "Hello from client";
    struct sockaddr_in     servaddr;
    char *notfound = "NOTFOUND";
    char *end = "END";

    // Create socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Fill server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 

    while(1) {
        char filename[MAXLINE] = {0}, ch;
        printf("Enter Filename. (Press Ctrl+C to end session)\n");
        scanf("%[^\n]c",filename);
        scanf("%c",&ch);

        int n, len;
        
        // send filename to server
        sendto(sockfd, (const char *)filename, strlen(filename), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        printf("Request for file \"%s\" sent\n",filename);
        
        char buffer[MAXLINE]={0};
        // store server response
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';
        
        // if server response is "NOTFOUND", exit
        if (strcmp(buffer,notfound) == 0) {
            printf("File Not Found\n");
            printf("End of Session\n");
            printf("---------------------\n");
            continue;
        }

        // if server response is "HELLO", do the following
        printf("Server:\t%s\n",buffer );

        // create a new file in write mode
        FILE *fptr = fopen("output.txt","w");
        if(fptr == NULL) {
            printf("Couldn\'t open output file.\n");
            sendto(sockfd, (const char *)"ABORT", strlen("ABORT"), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
            printf("End of Session\n");
            return -1;
        } else {
            sendto(sockfd, (const char *)"SUCCESS", strlen("SUCCESS"), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        }

        // send requests for WORD1, WORD2, WORD3, ...
        // write each server response to the output file
        // until server responds with END
        int i = 1;      // word count
        while (1) {
            char istr[20];
            sprintf(istr,"%d",i);

            // send request for ith word : "WORDi"
            char word[100] = "WORD";
            strcat(word,istr);
            printf(">>>>\tRequesting %s\n",word);
            sendto(sockfd, (const char *)word, strlen(word), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

            // store server response for "WORDi"
            char wordbuffer[MAXLINE] = {0};
            n = recvfrom(sockfd, (char *)wordbuffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
            wordbuffer[n] = '\0';
            printf("Server:\t%s\n",wordbuffer);
            
            // if "WORDi" is "END", exit. Otherwise write in output file
            if( strcmp(wordbuffer,end) == 0 ) {
                fclose(fptr);
                printf("%d words in %s\n",i-1,filename);
                printf("End of Session\n");
                printf("---------------------\n");
                break;
            } else {
                fprintf(fptr,"%s\n",wordbuffer);
            }
            
            i++;
        }
    }

    close(sockfd);
    return 0; 
}