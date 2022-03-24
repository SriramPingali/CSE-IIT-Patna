#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int child_pid = fork();

	// Parent process
	if (child_pid > 0) {
		exit(0);
	} else {
        // Child process
        sleep(50); 
    }
    
	return 0; 
}