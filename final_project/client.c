#include <arpa/inet.h> // internet socket address conversion funcs are defined
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // definitions of socket interface
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#define PORT 8080
#define MAX_THREADS 10
#define MAX_LEN 1024

int client_fd; // client file descriptor
char name[20];
char buffer[1024] = {0};

void signal_handler(int sig){
	if (sig == SIGINT){
  	printf("Are you sure you want to leave the chat? y/n");
		char answer;
    scanf("%c", &answer);
		if (answer == 'y'){
			printf("Goodbye\n");
			exit(0);
		}
		else{
			printf("continue playing\n");
		}
  }
}

void* send_message(void* param){
	signal(SIGINT, signal_handler);
	while(1){
	char text[MAX_LEN];
	char buf[MAX_LEN];
	printf("%s] ", name);
	fgets(text, MAX_LEN, stdin);

	snprintf(buf, MAX_LEN, "%s] %s\n", name, text);
	send(client_fd, buf, strlen(buf), 0);
	}
}

void* recv_message(void* param){
	int valread = read(client_fd, buffer, MAX_LEN - 1);
	printf("%s\n", buffer);
	return NULL;
}



int main(int argc, char* argv[]){
	int status; // status: status of socket connection valread: size of data coming from server, client file descriptor
	struct sockaddr_in serv_addr; // saving server address
	pthread_t send_thread;
	pthread_t receive_thread;

	// IPv4 TCP socket creation
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket creation error\n");
		return -1;
	}
	printf("Clients starts.\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// convert IPv4 server adresses from text to binary form, save it to sin_addr
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
		printf("Invalid address - Address not supported\n");
		return -1;
	}
	
	// connect with server
	if ((status = connect(client_fd,(struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("connection Failed\n");
		return -1;
	}
	else {
		printf("your name: ");
		if (fgets(buffer, sizeof(buffer), stdin)) {
    buffer[strcspn(buffer, "\n")] = '\0'; // delete enter
}
		printf("connection successful.\n");
		char join[MAX_LEN];
		snprintf(join, MAX_LEN, "%s joined.\n", name);
		send(client_fd, join, strlen(join), 0);
	}


	// communicate with server
	pthread_create(&receive_thread, NULL, recv_message, NULL);
	pthread_create(&send_thread, NULL, send_message, NULL);
	pthread_join(send_thread, NULL);
	pthread_join(receive_thread, NULL);

	close(client_fd);
	return 0;
}
