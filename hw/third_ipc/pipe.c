#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

//send message from parent to child, child prints received message
int main(){
	int pipefd[2]; //array saving pipe file descripter fd[0]: for reading fd[1]: for writing
	pid_t child_pid;
	char buff[5];
	char* message = "1234";

	//create pipe
	if (pipe(pipefd) == -1){ // if failed
		perror("pipe");  
		exit(EXIT_FAILURE); 
	}

	child_pid = fork(); // duplicate process

	if (child_pid == -1){ // if failed to fork
		perror("fork\n");
		exit(EXIT_FAILURE);
	}

	// write and read
	if (child_pid == 0){ // executing in child process
		close(pipefd[1]); // bcz no need to write

		while (read(pipefd[0], &buff, 4) > 0){ // read data in pipe
			buff[4] = '\0'; // make it string or it could get error
			printf("Child process : Received \"%s\" from parent\n", buff);
		}
		
		close(pipefd[0]); // done reading
		exit(EXIT_SUCCESS);
	}
	else { // executing in parent process
		close(pipefd[0]); // bcz no need to read
		printf("Parent process: send \"%s\" to child...\n", message);
		write(pipefd[1], message, strlen(message));
		close(pipefd[1]); // done writing
		wait(NULL); // wait til the end of child process
		exit(EXIT_SUCCESS);
	}
	return 0;
}
