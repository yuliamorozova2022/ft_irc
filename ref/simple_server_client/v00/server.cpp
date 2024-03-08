/* 
	This is a simple server that echoes whatever message the client sends.
	it accepts however many messages by the 'while' loop
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

const int PORT = 6666;

int main(int argc, char const *argv[])
{
	int server_fd, new_socket;
	ssize_t valread;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);
	std::vector<char> buf_vec(5000);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port
	if (setsockopt(server_fd, SOL_SOCKET,
				   SO_REUSEADDR | SO_REUSEPORT, &opt,
				   sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port specified
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int bytes;
	//accept incoming connection request from client
	new_socket = accept(server_fd, (struct sockaddr *)&address,
						&addrlen);
	while ((bytes = recv(new_socket, buf_vec.data(), buf_vec.size(), 0)) > 0)
	{
		std::cout << "Received " << bytes << " bytes: ";
		std::cout.write(buf_vec.data(), bytes);
		std::cout << std::endl;
	}
	std::cout << "Client disconnected." << std::endl;
	close(new_socket);

	// closing the listening socket
	close(server_fd);
	return 0;
}
