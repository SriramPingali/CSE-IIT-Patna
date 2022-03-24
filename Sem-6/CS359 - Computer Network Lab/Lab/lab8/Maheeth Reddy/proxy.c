/*
    Networks Lab 8

    Name: Maheeth Reddy
    Roll: 1801CS31

    TCP Proxy server
*/

// include all necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>

// define necessary constants
#define MAX 1024
#define N 1000
#define EXIT "exit"

int main(int argc, char *argv[]) {
    signal(SIGPIPE, SIG_IGN);

    // input validation
    if (argc != 4) {
        printf("Error: Incorrect command line arguments passed\n");
        printf("Try :\t./proxy <listen_port> <proxy_IP> <proxy_port>\n");
        return 0;
    }

    struct sockaddr_in proxyaddr, browaddr, cliaddr;

    browaddr.sin_family = AF_INET;
    browaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    browaddr.sin_port = htons(atoi(argv[1]));

    proxyaddr.sin_family = AF_INET;
    proxyaddr.sin_port = htons(atoi(argv[3]));

    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 1;
    
    int sock_in = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_in == 0) {
        perror("Socket in creation failed\n");
        exit(EXIT_FAILURE);
    }

    int param = fcntl(sock_in, F_SETFL, O_NONBLOCK);
    if (param == -1) {
        perror("Could not make non-blocking socket\n");
        exit(EXIT_FAILURE);
    }

    int sel = 1;
    int sso = setsockopt(sock_in, SOL_SOCKET, SO_REUSEADDR, &sel, sizeof(sel));
    if(sso != 0) {
        perror("Setsockopt failure for socket in\n");
        exit(EXIT_FAILURE);
    }

    int ipton = inet_pton(AF_INET, argv[2], &proxyaddr.sin_addr);
    if(ipton <= 0) {
        perror("Proxy Server Address is Incorrect\n");
        exit(EXIT_FAILURE);
    }

    int bi = bind(sock_in, (struct sockaddr*)&browaddr, sizeof(browaddr));
    if(bi != 0) {
        perror("Socket Bind failure\n");
        exit(EXIT_FAILURE);
    }

    if(listen(sock_in, N) < 0) {
        perror("Failed to Listen\n");
        exit(EXIT_FAILURE);
    }

    int FD, nselect;
    socklen_t len_cli;
    fd_set get_set, send_set;
    int fd_get[N], fd_send[N], count = 0;

    FD_ZERO(&get_set);
    FD_ZERO(&send_set);

    printf("Proxy running on port %s.\n", argv[1]);
    printf("All connections Forwarded to %s:%s\n", argv[2], argv[3]);

    while(1) {
        if(count < N) {
        	fd_get[count] = accept(sock_in, (struct sockaddr *)&cliaddr, &len_cli);
        	if(fd_get[count] >= 0) {
	            char str[100];
	        	
                fd_send[count] = socket(AF_INET, SOCK_STREAM, 0);
	            if(fd_send[count] == -1) {
	                fprintf(stdout, "socket() failed: %s\n", strerror(errno));
	                exit(0);
	            }

	            inet_ntop(AF_INET, &(cliaddr.sin_addr), str, 100);
	            printf("Connection accepted from %s:%d\n", str, (int) ntohs(cliaddr.sin_port));
	            int fnc = fcntl(fd_send[count], F_SETFL, O_NONBLOCK);
	            if (fnc == -1) {
	                fprintf(stdout, "fcntl() failed: %s\n", strerror(errno));
	                exit(EXIT_FAILURE);
	            }

	            // cannot connect immediately, needs some time
	            connect(fd_send[count], (struct sockaddr *)&proxyaddr, sizeof(proxyaddr));

	            count+=1;
	        }
	    }
        FD_ZERO(&get_set);
        FD_ZERO(&send_set);
        FD_SET(0,&get_set);
        FD = 1;
        // add all connections to FD_SET
        for(int i = 0; i < count; i++) {
            FD_SET(fd_get[i], &get_set);
            FD_SET(fd_send[i], &get_set);
            FD_SET(fd_get[i], &send_set);
            FD_SET(fd_send[i], &send_set);

            if(fd_get[i] > FD) {
            	FD = fd_get[i];
            }

            if(fd_send[i] > FD) {
            	FD = fd_send[i];
            }
        }
        FD += 1;

        nselect = select(FD, &get_set, &send_set, NULL, &timeout);
        if (nselect > 0) {
            char buffer[MAX] = {0};
            int p,q;
            
            if(FD_ISSET(0, &get_set) != 0) {
                p = read(0, buffer, sizeof(buffer));
                printf("\n-------- Exiting now --------\n");
				int len = strlen(buffer);
				
                if(buffer[len-1] == '\n') {
					buffer[len-1] = 0;
				}
				
                if(strcmp(buffer, EXIT) == 0) {
                	int i = 0;
					int num = sysconf(_SC_OPEN_MAX);
					for(i = 3; i < num; i++)
						close(i);

                    return 0;
                }
            }

            for(int i = 0; i < count; i++) {
                char buffer[MAX] = {0};
                int p, q;

                if(FD_ISSET(fd_get[i], &get_set) != 0) {
                	if(FD_ISSET(fd_send[i], &send_set) != 0) {
	                    p = read(fd_get[i], buffer, sizeof(buffer));
	                    q = send(fd_send[i], buffer, p, 0);
	                }
	            }

                if(FD_ISSET(fd_send[i], &get_set) != 0) {
                	if(FD_ISSET(fd_get[i], &send_set) != 0) {
	                    p = read(fd_send[i], buffer, sizeof(buffer));
	                    q = send(fd_get[i], buffer, p, 0);
                	}
                }
            }
        }
    }

    return 0;
}