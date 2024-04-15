#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

//send message from parent to child, child prints received message
int main(){
	int pipefd[2]; //array saving pipe file descripter fd[0]: for reading fd[1]: for writing
	pid_t child_pid;
	char buff;
	char* message = "1234\n";

	//create pipe
	if (pipe(pipefd) == -1){
		perror("pipe");  // when failed print error message
		exit(EXIT_FAILURE); 
	}

	child_pid = fork(); // duplicate process

	if (child_pid == -1){ // if failed to fork
		perror("fork\n");
		exit(EXIT_FAILURE);
	}

	// write and read
	if (child_pid == 0){ // excuting in child process
		close(pipefd[1]); // bcz no need to write

		while (read(pipefd[0], &buff, 1) > 0) // read data in pipe
			write(STDOUT_FILENO, &buff, 1); // STDOUT_FILENO means 표준 출력, = 1

		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]); // done reading
		exit(EXIT_SUCCESS);
	}
	else { // excuting in parent process
		close(pipefd[0]); // bcz no need to read
		write(pipefd[1], &buff, strlen(message));
		close(pipefd[1]); // done writing
		wait(NULL); // wait til the end of child process
		exit(EXIT_SUCCESS);
	}
	return 0;
}
