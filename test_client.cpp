// Client side C program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 6666

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status
		 = connect(client_fd, (struct sockaddr*)&serv_addr,
				   sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	send(client_fd, "message 1", strlen("message 1"), 0);
	printf("Hello 1 sent\n");
	
	if (send(client_fd, "message 2", strlen("message 1"), 0) == -1)
		printf("Hello 2 ERROR\n");
	else
		printf("Hello 2 sent\n");
	
	if (send(client_fd, "message 3", strlen("message 1"), 0) == -1)
		printf("Hello 3 ERROR\n");
	else
		printf("Hello 3 sent\n");

	// closing the connected socket
	close(client_fd);
	return 0;
}