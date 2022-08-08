// Name : Chandrawanshi Mangesh Shivaji 
// Roll No. : 1801CS16
// CS547 MIDSEM ASSIGNMENT 2022
// 24/02/2022 

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#define _GNU_SOURCE

int main()
{
    // Probable Signatures of a worm
    char signature1[] = "sftp_mkdir";
    char signature2[] = "ssh_userauth_password";
    char signature3[] = "sftp_write";
    char signature4[] = "ssh_channel_request_exec";
    int infected1 = 0, infected2 = 0, infected3 = 0, infected4 = 0;

    // DIR * similar to FILE *, can be used to open the directory
    DIR *d;
    struct dirent *dir;

    // Path to directory where search is to be done
    char path[]="./";
    char copy_of_path[50];
    strcpy(copy_of_path,path);

    d = opendir(path);

    if(d)
    {
        while((dir = readdir(d)) != NULL)
        {
            // reinitialize flags for signatures
            infected1 = 0, infected2 = 0, infected3 = 0, infected4 = 0;

            // Check whether the FILE selected is a .c file or not
            int len=strlen(dir->d_name);
            if(len>=2 && dir->d_name[len-2] == '.' && dir->d_name[len-1] == 'c'){

                strcat(path,"/");
                strcat(path, dir->d_name);
                FILE* f;
                f = fopen(path, "r");

                if(f == NULL){
                    printf("FILE NOT FOUND!\n");
                    continue;
                }

                // Check for all the signatures in every line
                char buffer[1024];
                while(fgets(buffer, 1024, f)){
                    buffer[strcspn(buffer,"\n")] = 0;

                    if(strstr(buffer, signature1)){
                        infected1 = 1;
                    }

                    if(strstr(buffer, signature2)){
                        infected2 = 1;
                    }

                    if(strstr(buffer, signature3)){
                        infected3 = 1;
                    }

                    if(strstr(buffer, signature4)){
                        infected4 = 1;
                    }
                }


                if(infected1 + infected2 + infected3 + infected4 >= 3){
                    printf("File %s may contain / may be WORM!!!\n", dir->d_name);
                } 

                fclose(f);
                strcpy(path, copy_of_path);

            }
        }
        closedir(d);
    }
    return 0;
}
