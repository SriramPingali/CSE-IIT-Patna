/*
    CS359 Computer Networks Lab
            Assignment 2
    
    Name: M. Maheeth Reddy
    Roll: 1801CS31

    >>>> Server Program <<<<
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT    8080 
#define MAXLINE 1024 

// function to check whether file is in current directory
int findFile(char *filename) { 
    struct dirent *de;
    DIR *dir = opendir("."); 

    if (dir == NULL) { 
        printf("Could not open current directory" ); 
        return 0; 
    } 

    while ((de = readdir(dir)) != NULL) {
        if (strcmp(filename,de->d_name) == 0) {
            return 1;
        }
    }
    
    closedir(dir);
    return 0; 
} 

int main() { 
    int sockfd;
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
    char *notfound = "NOTFOUND";
    char *found = "FOUND";
    char *end = "END";
    
    // Create socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("\nfile descriptor %d received\n", sockfd);
    }
    
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
    
    // Fill server information
    servaddr.sin_family      = AF_INET; // IPv4 address
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port        = htons(PORT);
    
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    } else {
        printf("\nSuccessfully binded!\n");
    }
    
    printf("\nPress Ctrl+C to stop the server\n\n");
    
    int len = sizeof(cliaddr);

    while(1) {
        char buffer[MAXLINE] = {0}, filename[MAXLINE]={0};

        // Get filename from client
        int n = recvfrom(sockfd, (char *)filename, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        filename[n] = '\0';
        printf("Client request for file \"%s\" received\n", filename);
        
        // If file not found, exit
        if(findFile(filename) == 0) {
            sendto(sockfd, (const char *)notfound, strlen(notfound), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            printf(">>>>\t%s\n",notfound);
            printf("End of Session\n");
            printf("---------------------\n");
            continue;
        }

        printf(">>>>\t%s\n",found);
        // open the file in read mode
        FILE *fptr = fopen(filename,"r");

        // if file could not be opened, exit
        if (fptr == NULL) {
            sendto(sockfd, (const char *)notfound, strlen(notfound), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            printf(">>>>\tFile \"%s\" %s\n",filename,notfound);
            continue;
        }

        // read first line of file ("HELLO") and send to client
        char fbuffer[MAXLINE+1];
        fscanf(fptr,"%s",fbuffer);
        sendto(sockfd, (const char *)fbuffer, strlen(fbuffer), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        printf(">>>>\tFirst line of file:\t%s\n",fbuffer);

        // if client can't open output file, exit
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';
        if (strcmp(buffer,"ABORT") == 0) {
            printf("Client can\'t open output file.\n");
            printf("End of Session\n");
            continue;
        }

        // send response for WORD1, WORD2, WORD3, ...
        // until END is encountered
        int i = 0;      // word count
        while (1) {
            char wordbuffer[MAXLINE] = {0};
            int n = recvfrom(sockfd, (char *)wordbuffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
            wordbuffer[n] = '\0';
            
            // read the word from file
            fscanf(fptr,"%s", fbuffer);
            i++;
            
            // send it to client
            sendto(sockfd, (const char *)fbuffer, strlen(fbuffer), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
                        
            // if word is "END", stop reading file
            if(strcmp(fbuffer,end) == 0) {
                fclose(fptr);
                printf("End of File\n");
                printf("End of Session\n");
                printf("------------------------\n");
                break;
            }
        }
    }

    return 0;
} 