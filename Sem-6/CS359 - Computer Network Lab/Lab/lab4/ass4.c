/*
		CS359 Assignment 4
	Peer-to-Peer Chat Application

	Name: M Maheeth Reddy
	Roll: 1801CS31
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>

#include <fcntl.h>
#include <errno.h>

#define BUFSIZE 1024
#define MAX 100
#define MAXMSG 500
#define USERS 5

typedef struct Line {
	char name[MAX];
	char message[MAXMSG];
} Line;

typedef struct User {
	char name[MAX];
	char ip[MAX];
	char port[MAX];
} User;

User user[USERS] = {
	{"frnd0", "127.0.0.1", "2020"},
	{"frnd1", "127.0.0.1", "2021"},
	{"frnd2", "127.0.0.1", "2022"},
	{"frnd3", "127.0.0.1", "2023"},
	{"frnd4", "127.0.0.1", "2024"}
};

// function find a character in a string
int findChar(char str[], char ch) {
	for(int i = 0; i < strlen(str); i++) {
		if(str[i] == ch) return 1;
	}
	return 0;
}

// extract name and message into struct Line
Line getNameMsg(char buffer[]) {
	Line temp;
	memset(&temp, 0, sizeof(temp));

	if(!findChar(buffer,'/')) {
		buffer[strlen(buffer)-1]='\0';
		strcpy(temp.name,buffer);
		strcpy(temp.message,"");
		return temp;
	}

	int cur;
	for(cur = 0; buffer[cur] != '/'; cur++) {
		temp.name[cur] = buffer[cur];
	}
	
	for(int i = cur+1; i < strlen(buffer); i++) {
		temp.message[i-cur-1] = buffer[i];
	}

	return temp;
}

// include name of sender along with message, before sending
char* sendNameMsg(char* buf, char name[]) {
	int done = 0;
	char message[MAXMSG] = {0};

	Line temp = getNameMsg(buf);

	strcpy(buf, name);
	strcat(buf,"/");
	strcat(buf, temp.message);
	buf[strlen(buf)] = '\0';

	return buf;
}

// print the user info table when starting the application
void printDetails() {
	printf("Users Information: \n");
	printf("\tID\tName\tIP Address\tPort\n");
	printf("\t------------------------------------\n");
	for (int i = 0; i < USERS; i++) {
		printf("\t%d\t%s\t%s\t%s\n", i, user[i].name, user[i].ip, user[i].port);
	}
	printf("\n");
}

int main() {
	printDetails();

	int id;
	while(1) {
		printf("Choose a user-id: ");
		scanf("%d",&id);
		if(id >= 0 && id <= 4) break;
		else printf("INVALID USER-ID: select your user-id from above list (from 0-4).\n");
	}

	// get user port, open socket for the application
	int port = atoi(user[id].port);
	int master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (master_socket < 0) {
		printf("ERROR: Can\'t create socket\n");
		exit(1);
	}
	
	// timeout for 1 second
	int optval = 1;
	setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

	struct sockaddr_in serveraddr;
	bzero((char *)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	if (bind(master_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
		printf("ERROR: Can\'t bind socket\n");
		exit(1);
	}

	if (listen(master_socket, USERS) < 0) {
		printf("Only 5 connections are possible!\n");
		exit(1);
	}

	// welcome message
	printf("\nWelcome \"%s\"!\n\tTo message a friend use the format: friendname/<msg>\n\tTo exit, type \"quit\"\nStart Chatting!\n\n", user[id].name);

	fd_set readset, tempset;
	FD_ZERO(&readset);
	FD_SET(master_socket, &readset);
	FD_SET(0, &readset);
	int allfd = master_socket;

	// set timeout
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;

	int exit1 = 0;

	while (1) {
		memcpy(&tempset, &readset, sizeof(tempset));

		// using select() system call
		int result = select(allfd+1, &tempset, NULL, NULL, &tv);
		if (result == 0) {
			if (exit1 == 1) {
				printf("Session ended\n");
				exit(1);
			}
		} else if (result < 0 && errno != EINTR) {
			printf("ERROR: select() returns negative value\n");
		} else if (result > 0) {
			if (FD_ISSET(master_socket, &tempset)) {
				socklen_t len = sizeof(serveraddr);
				int peersocket = accept(master_socket, (struct sockaddr *)&serveraddr, &len);
				if (peersocket < 0) {
					printf("ERROR: Can\'t accept connection on socket\n");
				} else {
					FD_SET(peersocket, &readset);
					allfd = (allfd > peersocket) ? allfd : peersocket;
				}
				FD_CLR(master_socket, &tempset);
			}

			if (FD_ISSET(0, &tempset)) {
				// read user input
				char buf[BUFSIZE];
				bzero(buf, sizeof(buf));
				if (read(0, buf, BUFSIZE) < 0) {
					printf("ERROR: Unable to read user input\n");
				}

				// if user typed quit, then end session
				if (strcmp(buf, "quit\n") == 0) {
					exit1 = 1;
					continue;
				}

				char temp[BUFSIZE];
				strcpy(temp, buf);
				temp[strlen(buf)] = '\0';
				// get name and message from the user input
				Line msg = getNameMsg(temp);

				// search for peer in info table
				int destport, validPeer = 0;
				char hostip[20];
				for (int i = 0; i < USERS; i++) {
					if (strcmp(user[i].name, msg.name) == 0) {
						strcpy(hostip, user[i].ip);
						destport = atoi(user[i].port);
						validPeer = 1;
						break;
					}
				}

				if (!validPeer) {
					printf("User \"%s\" doesn\'t exist\n",msg.name);
					continue;
				}

				// open connection to requested user
				int sockfd = socket(AF_INET, SOCK_STREAM, 0);
				if (sockfd < 0) {
					printf("ERROR: can\'t open connection to \"%s\"\n",msg.name);
					exit(1);
				}

				struct hostent* server = gethostbyname(hostip);
				if (server == NULL) {
					printf("ERROR: Server %s doesn\'t exist\n", hostip);
					exit(0);
				}

				struct sockaddr_in destaddr;
				bzero((char *)&destaddr, sizeof(destaddr));
				destaddr.sin_family = AF_INET;
				bcopy((char *)server->h_addr, (char *)&destaddr.sin_addr.s_addr, server->h_length);
				destaddr.sin_port = htons(destport);

				if (connect(sockfd, (struct sockaddr *)&destaddr, sizeof(destaddr)) < 0) {
					printf("ERROR: can\'t connect to \"%s\"\n",msg.name);
					exit(1);
				}

				// send message to requested user
				strcpy(buf, sendNameMsg(buf, user[id].name));
				int n = write(sockfd, buf, strlen(buf));
				if (n < 0) {
					printf("ERROR: can\'t write to socket\n");
					exit(1);
				}

				close(sockfd);
				continue;
			}

			// application is ready to receive message from any other user unless current user is typing something
			char buffer[BUFSIZE+1];
			for (int j = 1; j <= allfd; j++) {
				if (FD_ISSET(j, &tempset)) {
					bzero(buffer, sizeof(buffer));

					do {
						result = recv(j, buffer, BUFSIZE, 0);
					} while (result == -1 && errno == EINTR);

					if (result > 0) {
						buffer[result] = '\0';

						char temp2[BUFSIZE];
						strcpy(temp2, buffer);
						temp2[strlen(buffer)] = '\0';
						Line msg1 = getNameMsg(temp2);

						printf("%s : %s", msg1.name, msg1.message);
					} else if (result == 0) {
						close(j);
						FD_CLR(j, &readset);
					} else {
						printf("ERROR: can\'t receive messages\n");
					}
				}
			}
		}
	}

	return 0;
}