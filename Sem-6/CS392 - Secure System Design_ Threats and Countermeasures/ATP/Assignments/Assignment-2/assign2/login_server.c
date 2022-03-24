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

// check whether the given username exists in F1 file
int searchUser(char username[], FILE* f1, char sweetindices[]) {
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, f1)) != -1) {
        char useri[MAX] = {0};
        sscanf(line,"%s %[^\n]s",useri,sweetindices);
        useri[strlen(useri)-1] = '\0';
        // printf("useri: %s\t username: %s\n",useri,username);

        if(strcmp(useri,username) == 0) {
            // printf("sweetindices: %s\n",sweetindices);
            return 1;
        }
    }

    bzero(sweetindices,strlen(sweetindices));
    return 0;
}

// given the index, get the password hash from F2 file
int getHashbyIndex(FILE* f2, int index, char hash[]) {
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int _index;
    
    while ((read = getline(&line, &len, f2)) != -1) {
        for(int i = 0; i < strlen(line); i++) {
            if(line[i] == ':') {
                line[i] = ' ';
            }
        }
        
        sscanf(line,"%d %s",&_index,hash);
        // printf("%d:%s\n",_index,hash);
        if(index == _index) return 1;
    }

    return 0;
}

// check whether this username belongs to a honeypot account
int isHoneypot(char username[]) {
    FILE *f4 = fopen("F4.txt","r");
    
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int _index;
    
    char userHash[MAX] = {0};
    while ((read = getline(&line, &len, f4)) != -1) {
        line[strlen(line)-1] = '\0';

        computeMD5(username,userHash);
        if(strcmp(userHash,line) == 0) return 1;
    }

    return 0;
}

// check whether the given password is valid or not
int checkPassword(char username[], int sweetX[], int k, char hash[]) {
    FILE* f2 = fopen("F2.txt","r");
    
    for(int i = 0; i < k; i++) {
        char f2hash[MAX] = {0};
        getHashbyIndex(f2,sweetX[i],f2hash);

        if(strcmp(hash,f2hash) == 0) {
            if(isHoneypot(username)) {
                // report breach directly
                return -1;
            } else {
                // send <username,i> to honeychecker
                return sweetX[i];
            }
        }
    }

    // if no sweet-index matches, login fails
    return -2;
}

int main() {
    // create connection with honeychecker
    char *ip = "127.0.0.1";
    int port = 8080;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    int e = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(e == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Honeychecker.\n");

    // ask for login or signup
    int mode = 0;
    printf("Enter 0 for login, 1 for signup: ");
    scanf("%d",&mode);
    send(sockfd, &mode, sizeof(mode), 0);

    // get the inputs
    char username[MAX] = {0};
    char password[PASS_LENGTH] = {0};

    printf("Enter details:\n");
    printf("Username:\t");
    scanf("%s",username);
    printf("Password:\t");
    scanf("%s",password);

    char hash[MAX] = {0};
    computeMD5(password,hash);

    if(mode == 0) {
        // login
        FILE* f1 = fopen("F1.txt","r");
        FILE* f2 = fopen("F2.txt","r");
        
        // if user doesn't exist, show error
        char sweetindices[MAX] = {0};
        if(!searchUser(username,f1,sweetindices)) {
            printf("Wrong username or password entered\n");
            char msg[MAX] = "-2 Wrong username entered\n";
            send(sockfd, msg, sizeof(msg), 0);
            return 1;
        }

        // extract honeyindex set
        int sweetX[MAX];
        int i = 0, k = 6, n = 0;
        char *cur = sweetindices;
        for(int i = 0; i < k; i++) {
            sscanf(cur,"%d%n",&sweetX[i],&n);
            // printf("\t%d\n",sweetX[i]);
            cur += n;
        }
        printf("\n");

        // check validity of password and send relevant response to honeychecker
        int res = checkPassword(username,sweetX,k,hash);
        if(res == -2) {
            printf("Wrong username or password entered\n");
            char msg[MAX] = "-2 Wrong password entered\n";
            send(sockfd, msg, sizeof(msg), 0);
            return 1;
        } else if(res == -1) {
            printf("Login denied\n");
            char msg[MAX] = "-1 Login attempt from honeypot account\n";
            send(sockfd, msg, sizeof(msg), 0);
            return 1;
        } else {
            char user_index[MAX] = {0};
            sprintf(user_index,"%d %s",res,username);
            send(sockfd, user_index, sizeof(user_index), 0);

            int response = 0;
            recv(sockfd, &response, sizeof(response), 0);

            if(response == 1) {
                printf("login successful\n");
            } else {
                printf("login unsuccessful\n");
            }
        }

        printf("[+]Closing the connection.\n");
        close(sockfd);
    } else if(mode == 1) {
        // signup
        printf("Signing up\n");
        send(sockfd, username, MAX, 0);
        send(sockfd, password, PASS_LENGTH, 0);
    } else {
        printf("Invalid Input!\n");
        return 1;
    }
}