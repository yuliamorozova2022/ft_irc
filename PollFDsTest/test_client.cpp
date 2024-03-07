// Improved version of a simple client. this one should be able to listen to incoming messages
//	from the server while having STDIN open to send messages.

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <poll.h>
#include <vector>
#include <cstdlib>

#include "./PollManager.hpp"

#define PORT 6666
#define MAX_CLIENTS 30
#define POLL_TIMEOUT (60 * 1000) // 1 min, in milliseconds

int socket_setup ()
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	int client_fd;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket creation Failed ");
		exit (EXIT_FAILURE);
	}
	if (connect(client_fd, (struct sockaddr*)&serv_addr,
			sizeof(serv_addr)) < 0)
	{
		perror("Connection Failed ");
		exit (EXIT_FAILURE);
	}

	return client_fd;
}

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	char buffer[1024] = { 0 };
	std::string msg;
	std::vector<char> buf_vec(5000);

	client_fd = socket_setup();

//SETTING UP POLL
	std::cout << "HERE" << std::endl;

	PollManager	fds(2);


	//setup first fd for stdin, if data is read, send it to the server
	fds.addFD(STDIN_FILENO);
	//Then for clientfd, which is where messages from server would be accepted
	fds.addFD(client_fd);

	int poll_status;
	int stat;
	bool allok = true;
	while(allok)
	{
		poll_status = poll(fds.getFds(), fds.getSize(), POLL_TIMEOUT);
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

			if(fds[i].revents == 0) //not the called event
				continue;
			if(fds[i].revents != POLLIN)
			{
				printf("  Error! revents = %d\n", fds[i].revents);
				allok = false;
				break;
			}
			if (fds[i].fd == STDIN_FILENO) // called on stdin
			{
				stat =  read(fds[i].fd, buf_vec.data(), buf_vec.size());
				if (stat < 1)
				{
					perror("ERROR");
					allok = false;
					break;
				}
				buf_vec.data()[strlen(buf_vec.data()) - 1] = 0;
				// printf("{%s}\n", buf_vec.data());
				stat = send(client_fd, buf_vec.data(), strlen(buf_vec.data()), 0);
				if (stat == -1)
				{
					perror("ERROR");
					allok = false;
					break;
				}

			}

			else if (fds[i].fd == client_fd) //called on connection fds
			{
				stat = recv(fds[i].fd, buf_vec.data(), buf_vec.size(), 0);
				if (stat < 1)
				{
					allok = false;
					break;
				}
				std::cout	<< "Res from server: "
					<< strlen(buf_vec.data())
					<< " {"
					<< buf_vec.data()
					<< "}" << std::endl;

			}
		}
	}
	// closing the connected socket
	close(client_fd);
	return 0;
}