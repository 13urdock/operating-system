#include <stdio.h>
#include <unistd.h>

int g_val = 10; // when forked same data are duplicated

void do_child(){
	g_val++; // after fork, global values are not shared bcz they are independent
	printf("child process g_val = %d\n", g_val);	
}

void do_parent(){
	printf("parent process g_val = %d\n", g_val);
}

int main(int argc, char* argv[]){
	pid_t pid = fork();
	if (pid < 0){
		fprintf(stderr, "Fork failed");
		return 1;
	}
	else if (pid == 0){
		do_child();
	}
	else{
		wait(NULL); // wait until child ends
		do_parent();	
	}

	return 0;
}
