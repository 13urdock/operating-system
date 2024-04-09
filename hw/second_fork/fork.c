#include <sys/types.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>

int main(){
	pid_t pid; // process id(int)

	for (int i = 0; i < 2; i++){
		pif = fork(); // fork child process

		if (pid < 0){ // failed to create child process
			printf("Fork Failed");
		}
		else if (pid == 0){ // child process(pid = 0)
			printf("Hello from child in %d loop\n", i);
		}
		else{ // parent process
			wait(NULL);
			printf("Hello from parent in %d loop\n", i);
		}
		printf("%d loop\n", i);
	}

	return 0;
}


