#include <arpa/inet.h> // internet socket address conversion funcs are defined
#include <stdio.h>
#include <stdlib.h> // definitions of socket interface
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#define PORT 8080
#define MAX_THREADS 10
#define MAX_LEN 1024

int client_fd; // client file descriptor
char* name;
char buffer[MAX_LEN] = {0};

void signal_handler(int sig) {
	if (sig == SIGINT) {
		printf("Are you sure you want to leave the chat? y/n");
    char answer;
    scanf(" %c", &answer);
    if (answer == 'y') {
    	printf("Goodbye\n");
      exit(0);
		}
		else {
			return;
		}
  }
}

void* send_message(void* param) {
  while (1) {
    char text[MAX_LEN];
    char buf[MAX_LEN];
    printf("%s] ", name);
    if (fgets(text, MAX_LEN, stdin) == NULL) {
      break;
    }
    text[strcspn(text, "\n")] = '\0'; // remove newline character
    snprintf(buf, MAX_LEN, "%s] %s\n", name, text);
    send(client_fd, buf, strlen(buf), 0);
  }
  return NULL;
}

void* recv_message(void* param) {
  int valread;
  while (1) {
    valread = read(client_fd, buffer, MAX_LEN - 1);
    if (valread > 0) {
      printf("%s", buffer);
    } 
		else if (valread == 0) {
      printf("Connection closed by server\n");
      break;
    } 
		else {
      perror("read");
      break;
    }
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  int status;// status: status of socket connection
  struct sockaddr_in serv_addr;// saving server address

	
	signal(SIGINT, signal_handler); // register signal handler

	// IPv4 TCP socket creation
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("socket creation error\n");
    return -1;
  }
  printf("Client starts.\n");
  
	serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // convert IPv4 server adresses from text to binary form, save it to sin_addr
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("Invalid address - Address not supported\n");
    return -1;
  }
	
	//connect with server
  if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
    printf("connection Failed\n");
    return -1;
  } 
	else {
    name = malloc(MAX_LEN); // dynamically allocate memory for name
    printf("your name: ");
    if (fgets(name, MAX_LEN, stdin)) {
      name[strcspn(name, "\n")] = '\0'; // remove newline character
  	}
    printf("connection successful.\n");
    send(client_fd, name, strlen(name), 0);
  }

	// communicate with server
  pthread_t send_thread, receive_thread;
  pthread_create(&receive_thread, NULL, recv_message, NULL);
  pthread_create(&send_thread, NULL, send_message, NULL);
  pthread_join(send_thread, NULL);
  pthread_join(receive_thread, NULL);

	free(name); // free dynamically allocated memory
 	close(client_fd);
  return 0;
}
