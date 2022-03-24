#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
  
int global_var = 0; 


//incrementtion to increase static and global variable value
void *increment() { 
    static int local_var = 0; //static local_var variable
    local_var++;
    global_var++;
  
  	pthread_exit(&local_var);
} 


int main() { 
    int i,ret; 
    pthread_t id[10]; 
    int loc;
    void *exit_status;

    for (int i = 0; i < 10; i++) {
        ret = pthread_create(&id[i], NULL,increment, NULL);
        if(ret){
            printf("ERROR: thread creation failed!! return code is %d \n", ret);
            exit(1); 
        }
    }
   
   for(int i = 0; i < 10; i++){
        pthread_join(id[i],&exit_status);
    }

   	printf("Static: %d, Global: %d\n",*(int*)exit_status, global_var);
   
    pthread_exit(NULL); 
    return 0; 
}