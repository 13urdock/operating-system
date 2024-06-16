#include <arpa/inet.h> // internet socket address conversion funcs are defined
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> // definitions of socket interface
#include <unistd.h>
#define PORT 8080

int main(int argc, char* argv[]){
	int status, valread, client_fd; // status: status of socket connection valread: size of data coming from server, client file descriptor
	struct sockaddr_in serv_addr; // saving server address
	char* hello = "Hello from client\n";
	char buffer[1024] = {0};

	// IPv4 TCP socket creation
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket creation error\n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// convert IPv4 server adresses from text to binary form, save it to sin_addr
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
		printf("Invalid address - Address not supported\n");
		return -1;
	}
	
	// connect with server
	if((status = connect(client_fd,(struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("connection Failed\n");
		return -1;
	}

	// communicate with server
	send(client_fd, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	for (int i = 0; i < 4; i++){
		valread = read(client_fd, buffer, 1024 - 1); // substract 1 for the null terminator at the end
		printf("%s\n", buffer);
	}

	close(client_fd);
	return 0;
}
