#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 1000
#define SIZE 1048576
#define PASS_LENGTH 13

// function to compute md5sum
void computeMD5(char *str, char *md5){
    char cmd[SIZE] = {0};
    
    strcpy(cmd,"echo -n \"");
    strcat(cmd,str);
    strcat(cmd,"\" | md5sum");
    
    FILE* fp = popen(cmd, "r");
    fscanf(fp,"%s", md5);
    pclose(fp);
}

// get sugarindex for user from F3 file
int getSugarIndex(char username[]) {
    FILE* f3 = fopen("F3.txt","r");
    
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char md5[MAX] = {0}, md5user[MAX] = {0};
    computeMD5(username,md5user);
    while ((read = getline(&line, &len, f3)) != -1) {
        sscanf(line,"%s",md5);
        md5[strlen(md5)-1] = '\0';

        if(strcmp(md5,md5user) == 0) {
            int sugarIndex;
            sscanf(line+strlen(md5)+1,"%d",&sugarIndex);
            // printf("sugarIndex: %d\n",sugarIndex);
            return sugarIndex;
        }
    }
}

// get current number of users from F1 file
int numUsers(FILE* f2) {
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    while ((read = getline(&line, &len, f2)) != -1) {
        line[strlen(line)-1] = '\0';
        count++;
    }
    return count;
}

// linear search
int isPresent(int arr[], int n, int elt){
    for(int i = 0; i < n; i++) {
        if(arr[i] == elt) return 1;
    }
    return 0;
}

// make honeyindex set for new user
void assignSweets(int sweetX[], int k, int users) {
    for(int i = 0; i < k; i++) sweetX[i] = -1;

    int index = rand()%k;
    sweetX[index] = users;

    while(isPresent(sweetX,k,-1)) {
        index = rand()%k;
        int sweet = rand()%users + 1;
        while(1) {
            if(!isPresent(sweetX,k,sweet)) {
                sweetX[index] = sweet;
                break;
            } else {
                sweet = rand()%users + 1;
            }
        }
    }
}

int main(){
    char *ip = "127.0.0.1";
    int port = 8080;

    // make connection with client
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
        printf("[+]Waiting for communication from login server....\n");
    } else {
        perror("[-]Error in listening");
        exit(1);
    }
    printf("\n");
    
    socklen_t addr_size = sizeof(new_addr);
    int new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    
    // get login/signup mode from login server
    int mode = 0, k = 6;
    recv(new_sock, &mode, sizeof(mode), 0);

    if(mode == 0) {
        // login
        printf("Login Attempt detected\n");
        char buffer[MAX] = {0};
        recv(new_sock, buffer, MAX, 0);
        
        int res;
        char username[MAX] = {0};
        sscanf(buffer,"%d %s",&res,username);
        
        // if wrong username or password, print relevant message
        // else, send sugarindex to login server
        if(res == -2) {
            printf("%s\n",buffer+3);
        } else if(res == -1) {
            printf("%s\n",buffer+3);
        } else if(res >= 0) {
            int sugarindex = getSugarIndex(username);
            int resp = (res == sugarindex);
            send(new_sock, &resp, sizeof(resp), 0);
            if(resp == 1) {
                printf("%s has logged in successfully\n",username);
            } else {
                printf("%s\'s login attempt failed\n",username);
            }
        }
    } else if(mode == 1) {
        // signup
        printf("Signup Attempt detected\n");
        char username[MAX] = {0}, password[MAX] = {0};
        recv(new_sock, username, MAX, 0);
        recv(new_sock, password, PASS_LENGTH, 0);
        printf("received %s:%s\n",username,password);

        // update details in F1 F2 F3 files
        FILE* f1 = fopen("F1.txt","r");
        if(!f1) {
            printf("Can\'t read F1 file\n");
            return 1;
        }

        int users = numUsers(f1);
        users++;
        fclose(f1);

        int sweetX[k];
        assignSweets(sweetX,k,users);

        f1 = fopen("F1.txt","a");
        if(!f1) {
            printf("Can\'t update F1 file\n");
            return 1;
        } else printf("updating F1 file");

        fprintf(f1,"\n");
        fprintf(f1,"%s: ",username);
        for(int i = 0; i < k; i++) {
            fprintf(f1,"%d",sweetX[i]);
            if(i < k-1) fprintf(f1," ");
        }

        FILE* f2 = fopen("F2.txt","a");
        if(!f2) {
            printf("Can\'t update F2 file\n");
            return 1;
        }
        char md5pass[SIZE] = {0};
        computeMD5(password,md5pass);
        fprintf(f2,"%d:%s\n",users,md5pass);

        FILE* f3 = fopen("F3.txt","a");
        if(!f3) {
            printf("Can\'t update F3 file\n");
            return 1;
        }
        char md5user[SIZE] = {0};
        computeMD5(username,md5user);
        fprintf(f3,"%s: %d\n",md5user,users);

        // send();
        fclose(f1);
        fclose(f2);
        fclose(f3);
    }

    printf("\n[+]Closing the connection.\n");

    close(new_sock);
    close(sockfd);
    return 0;
}