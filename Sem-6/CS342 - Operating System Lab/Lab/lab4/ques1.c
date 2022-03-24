#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() { 
    int child_pid = fork();

	// Parent process
	if (child_pid > 0) {
        sleep(50);
	} else {
        // Child process
		exit(0);
    }
    
	return 0;
}