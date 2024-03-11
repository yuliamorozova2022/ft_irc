/*
	Attemp for an improved server which would accept multiple clients using 'poll'
	rn server accepts multple connections.

	Created a custom class to handle pollfds

 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cerrno>
#include <csignal>

#include "./PollManager.hpp"

// const int PORT = 6666;
const int POLL_TIMEOUT = 60 * 1000; // 1 min, in milliseconds

int setup_socket()
{
	int opt = 1;
	int server_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

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

	return (server_fd);
}

bool accept_new_connection(int server_fd, PollManager &fds)
{
	int new_connection;
	new_connection = accept(server_fd, NULL, NULL);
	if (new_connection < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			perror("  accept() failed");
			return false;
		}
	}
	else //add new connection to fds
	{
		printf("  New incoming connection - %d\n", new_connection);
		fds.addFD(new_connection);
	}
	return true;
}


int main(int argc, char const *argv[])
{
	int server_fd;
	int bytes;
	std::vector<char> buf_vec(5000);

	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	server_fd = setup_socket();
	int new_connection;

	//SIGNALS TEST
		// signal(SIGINT, sig_handler);
		// signal(SIGQUIT, sig_handler);


	//SETTING UP POLL
	PollManager			fds;

	//setup first fd for serverfd, where poll will listen for incoming connections
	fds.addFD(server_fd);

	int poll_status;
	int stat;
	bool allok = true;
	while(allok)
	{
		poll_status = fds.poll(POLL_TIMEOUT);
		if (poll_status < 0)
		{
			perror("  poll() failed");
			break;
		}
		else if (poll_status == 0)
		{
			printf("  poll() timed out.  End program.\n");
			break;
		}
		//otherwise, some event was called on one of the fds. check all of them
		for (int i = 0; i < fds.getSize(); i++)
		{
			memset(buf_vec.data(), 0 ,4000);

			if(fds.getFds()[i].revents == 0) //not the called event
				continue;
			if(fds.getFds()[i].revents != POLLIN)
			{
				printf("  Error! revents = %d\n", fds[i].revents);
				allok = false;
				break;
			}
			if (fds.getFds()[i].fd == server_fd) // called on serverfd
			{
				allok = accept_new_connection(server_fd, fds);
				if (!allok)
					break;
			}
			else //called on connection fds
			{
				printf("From Descriptor %d :\n", fds[i].fd);
				stat = recv(fds.getFds()[i].fd, buf_vec.data(), buf_vec.size(), 0);
				if (stat < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						perror("  recv() failed");
						allok = false;
					}
					break;
				}
				else if (stat == 0)
				{
					printf("  Connection closed\n");
					close(fds[i].fd);
					fds[i].fd = -1;
					break;
				}
				else
				{
					std::cout	<< strlen(buf_vec.data())
								<< "{"
								<< buf_vec.data()
								<< "}" << std::endl;


					stat = send(fds[i].fd, buf_vec.data(), strlen(buf_vec.data()), 0);
					printf("STAT: %i\n", stat);
					if (stat == -1)
						perror("ERROR");
				}

			}
		}
	}
	return 0;
}
