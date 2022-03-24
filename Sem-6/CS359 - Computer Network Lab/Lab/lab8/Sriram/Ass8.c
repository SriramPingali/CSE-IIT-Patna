// Set the browser Proxy IP to your machine IP 
// set the browser proxy to 8181(preferred) and use same in command line. 
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include<string.h>
#define BUFF_SIZE 1024
#define  n  1000

int main(int argc, char *argv[]) 
{

    signal(SIGPIPE, SIG_IGN);

    if (argc != 4) 
    {
        printf("Error: Incorrect command line arguments passed\n");
        printf("Try :\t./SimProxy <listen port> <institute_proxy_IP> <institute_proxy_port>\n");
        return 0;
    }


    struct sockaddr_in instiaddr, browaddr, cliaddr;
    socklen_t len, len_cli;

    int sel = 1,sock_in, FD, nselect;

    bzero(&browaddr, sizeof(browaddr));
    bzero(&instiaddr, sizeof(instiaddr));

    fd_set get_set, send_set;
    

    int fd_get[n], fd_send[n],count = 0;

    char buff[BUFF_SIZE];
    bzero(&buff, sizeof(buff));
    sock_in = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock_in == 0)
    {
        perror("Socket in creation failed\n");
        exit(EXIT_FAILURE);
    }
    int param = fcntl(sock_in, F_SETFL, O_NONBLOCK);
    if ( param == -1) 
    {
        perror("Could not make non-blocking socket\n");
        exit(EXIT_FAILURE);
    }
    int sso = setsockopt(sock_in, SOL_SOCKET, SO_REUSEADDR, &sel, sizeof(sel));
    if(sso!=0)
    {
        perror("Setsockopt failure for socket in\n");
        exit(EXIT_FAILURE);
    }

    browaddr.sin_family = AF_INET;
    browaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    browaddr.sin_port = htons(atoi(argv[1]));

    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 1;


    instiaddr.sin_family = AF_INET;
    instiaddr.sin_port = htons(atoi(argv[3]));
	int ipton = inet_pton(AF_INET, argv[2], &instiaddr.sin_addr);

    
    if ( ipton <= 0)
    {
        perror("Proxy Server Address Incorrect\n");
        exit(EXIT_FAILURE);
    }

    int bi = bind(sock_in, (struct sockaddr*)&browaddr, sizeof(browaddr));
    if (bi!=0)
    {
        perror("Socket Bind failure\n");
        exit(EXIT_FAILURE);
    }


   
    if (listen(sock_in, n) < 0)
    {
        perror("Failed to Listen\n");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&get_set);
    FD_ZERO(&send_set);

    printf("Proxy running on port %s.\n",argv[1]); 
    printf("All connections Forwarded to %s:%s\n",  argv[2], argv[3]);

    while (99) 
    {

        if (count < n  ) 
        {	
        	fd_get[count] = accept(sock_in, (struct sockaddr *)&cliaddr, &len_cli);
        	if(fd_get[count]>=0)
	        {
	            char str[100]; 
	        	fd_send[count] = socket(AF_INET, SOCK_STREAM, 0);
	            if ( fd_send[count] == -1) 
	            {
	                fprintf(stdout, "socket() failed: %s\n", strerror(errno));
	                exit(0);
	            }

	            inet_ntop(AF_INET, &(cliaddr.sin_addr), str, 100);	                   
	            printf("Connection accepted from %s:%d\n", str, (int) ntohs(cliaddr.sin_port));
	            int fnc = fcntl(fd_send[count], F_SETFL, O_NONBLOCK);
	            if (fnc == -1) 
	            {
	                fprintf(stdout, "fcntl() failed: %s\n", strerror(errno));
	                exit(EXIT_FAILURE);
	            }

	            // cannot connect immediately, needs some time
	            connect(fd_send[count], (struct sockaddr *)&instiaddr, sizeof(instiaddr));

	            count+=1;
	        }
	    }    
        FD_ZERO(&get_set);
        FD_ZERO(&send_set);
        FD_SET(0, &get_set);
        FD = 1;
        // add all connections to FD SET
        for (int i = 0; i < count; i++) 
        {
            FD_SET(fd_get[i], &get_set);
            FD_SET(fd_send[i], &get_set);
            FD_SET(fd_get[i], &send_set);
            FD_SET(fd_send[i], &send_set);
            if(fd_get[i]>FD)
            	FD =  fd_get[i];
            if(fd_send[i]>FD)
            	FD =  fd_send[i];
        }
        FD+=1;


        nselect = select(FD, &get_set, &send_set, NULL, &timeout);
        if (nselect > 0) 
        {
            char buff[BUFF_SIZE];
            int p, q;
            if (FD_ISSET(0, &get_set)!=0) 
            {
                memset(buff, 0, sizeof(buff));
                p = read(0, buff, sizeof(buff));
                printf("\ntyped: %s\n", buff);
				int len = strlen(buff);
				if(buff[len-1] == '\n'){
					buff[len-1] = 0;
				}
				char str[] = "exit";
                if (strcmp(buff, str) == 0) 
                {
                	int i=0;
					int num = sysconf(_SC_OPEN_MAX);
					for(i = 3; i < num; i++)
						close(i);

                    return 0;
                } 
            }
            for (int i = 0; i < count; i++) 
            {

                char buff[BUFF_SIZE];
                int p, q;

                if(FD_ISSET(fd_get[i], &get_set)!=0 )
                {
                	if(FD_ISSET(fd_send[i], &send_set)!=0) 
                	{
	                    memset(buff, 0, sizeof(buff));
	                    p = read(fd_get[i], buff, sizeof(buff));
	                    q = send(fd_send[i], buff, p, 0);
	                    if (q == -1 || p==0) 
	                    {
	                        if (errno == EPIPE) 
	                    	{
								// data is getting corrputed if we uncomment this
	                            //close(fd_get[i]);
	                            //close(fd_send[i]);
	                            //count-=1;
	                        }
	                    }
	                    // if (errno == EPIPE) 
	                    // {
	                    //     continue;
	                    // }
	                }
	            } 

                if(FD_ISSET(fd_send[i], &get_set)!=0 )
                {	
                	if( FD_ISSET(fd_get[i], &send_set)!=0) 
                	{
	                    memset(buff, 0, sizeof(buff));
	                    p = read(fd_send[i], buff, sizeof(buff));
	                    q = send(fd_get[i], buff, p, 0);
	                    if (q == -1 || p==0) 
	                    {
	                        if(errno == EPIPE) 
	                        {
								// data getting corrupted if we uncomment this
	                            //close(fd_get[i]);
	                            //close(fd_send[i]);
	                            //count-=1;
	                        }
	                    }
	                    // if (errno == EPIPE) 
	                    // {
	                    //     continue;
	                    // }
                	}
                }	
            }
        }

    }

    return 0;
}