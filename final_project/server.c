#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char* argv[]){
	int server_fd, user[3];
	ssize_t valread; // message from client
	struct sockaddr_in address; // server address
	int opt = 1; // for setsockopt. option value
	socklen_t addrlen = sizeof(address); //size of address
	char buffer[1024] = {0};	// buffer for message from client
	char* hello = "This is chatroom\n"; // message that will be sent to client

	// create socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket failed\n");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8000
	// SOL_SOCKET: protocol levet to set socket option
	// SO_REUSEADDR: you can use same port number after server process ended
	// SO_REUSEPORT: make several sockets share same port -> load balancing
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt\n");
		exit(EXIT_FAILURE);
	}	
	// server address saving in struct address
	address.sin_family = AF_INET; // address system(IPv4);
	address.sin_addr.s_addr = inet_addr("127.0.0.1"); // address of server IP (binding all IP address) set access acception range 
	address.sin_port = htons(PORT); // port number of server

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { // if success returns 0 or -1
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	// server socket turns receiving message mode. don't need while loop it keeps listening after execution
	// server socket discriptor, maximum number of waiting connection requirement
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	// create new socket and accept client's requirement
	for(int i = 0;i < 3; i++){
		if ((user[i] = accept(server_fd, (struct sockaddr*)&address, &addrlen))< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		else
			send(user[i], hello, strlen(hello), 0);
	}

	// communicate with client(user)
	for(int i = 0;i < 3; i++){
		valread = read(user[i], buffer, 1024 - 1); // subtract 1 for the null
		// terminator at the end
		printf("read and send%s\n", buffer);
		// sending messages to all clients
		for (int i = 0; i < 3; i++)
			send(user[i], buffer, strlen(buffer), 0);

		// printf("Hello message sent\n");
	}

	// closing the connected socket
	for (int i = 0; i < 3; i++)
		close(user[i]);

	// closing the listening socket
	close(server_fd);
	return 0;
