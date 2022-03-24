#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 1000
#define TIME 50

// function to swap two strings
void swap(char* s1, char* s2) {
    char temp[MAX] = {0};
    strcpy(temp,s1);
    strcpy(s1,s2);
    strcpy(s2,temp);
}

// mergesort helper function
void mergeSortedSubArrays(char contents[][MAX], int start, int mid, int end) {
    char temp[MAX][MAX];
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            temp[i][j] = 0;
        }
    }

    int index = start;
    for(int i = start, j = mid; (i < mid || j < end); index++) {
        if(i < mid && j < end) {
            if(strcmp(contents[i],contents[j]) > 0) {
                strcpy(temp[index],contents[i]);
                i++;
            } else {
                strcpy(temp[index],contents[j]);
                j++;
            }
        } else if (i < mid) {
            strcpy(temp[index],contents[i]);
            i++;
        } else {
            strcpy(temp[index],contents[j]);
            j++;
        }
    }

    for(int i = start; i < end; i++) {
        strcpy(contents[i],temp[i]);
    }
}

// mergesort function to sort array of strings in reverse order
void mergeSort(char contents[][MAX], int start, int end) {
    if(end == start+1) return;
    int mid = (start+end)/2;

    mergeSort(contents,start,mid);
    mergeSort(contents,mid,end);
    mergeSortedSubArrays(contents,start,mid,end);
}

int main(int argc, char *argv[]) {
    // PIDs for forked processes
	pid_t pid1, pid2;

	pid1 = fork();

    // child process-1 : Copy content of file1 to file2
	if(pid1 == 0) {
        printf("Child1\tPID=%d Parent-ID=%d\n",getpid(),getppid());
        printf("Copying contents of file1.txt to file2.txt\n");
        printf("------------------------------------------\n\n");

        FILE* fp1 = fopen("file1.txt","r");
        FILE* fp2 = fopen("file2.txt","w");
        char c = fgetc(fp1); 
        while (c != EOF) { 
            fputc(c, fp2); 
            c = fgetc(fp1);
        }

        fclose(fp1);
        fclose(fp2);
        exit(0);
	} else {
        // wait for child-1 to complete
        wait(NULL);
        
	    pid2 = fork();
        // child process-2 : Display content of file2
	    if (pid2 == 0) {
            printf("Child2\tPID=%d Parent-ID=%d\n",getpid(),getppid());
            printf("Displaying contents of file2.txt\n");
            printf("--------------------------------\n");

	    	FILE* fp2 = fopen("file2.txt","r");
            char c = fgetc(fp2); 
            while (c != EOF) { 
                printf ("%c", c); 
                c = fgetc(fp2); 
            }
            printf("\n\n");

            fclose(fp2);
            exit(0);
	    } else {
	        // child process-3 : Display sorted content of file2 in reverse order
            wait(NULL);

            printf("Child3\tPID=%d Parent-ID=%d\n",getpid(),getppid());
            printf("Displaying Sorted Content of file2 in reverse order\n");
            printf("---------------------------------------------------\n");

            FILE *fp = fopen("file2.txt","r");
            char contents[MAX][MAX];
            int count = 0;
            
            for(int i = 0; i < MAX; i++) {
                for(int j = 0; j < MAX; j++) {
                    contents[i][j] = 0;
                }
            }

            // read contents of file2.txt
            while (!feof(fp)) {
                fscanf(fp,"%s",contents[count]);
                count++;
            }
            
            // sort contents in reverse order
            mergeSort(contents,0,count);
            
            // print reversed contents
            for (int i = 0; i < count; i++) {
                printf("%s ",contents[i]);
            }
            printf("\n");
            
            fclose(fp);
	    }
	}
    
	return 0;
}