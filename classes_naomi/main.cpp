#include "includes.hpp"


// a beginner server, that will allow connection only with a PASS messagefrom user.
//todo: - some err for worong pass, and actually terminate connection
//		- timeout if user doesnt send pass packet?
//		- split main to smaller functions
//		- merge PollManager into Server?



const int POLL_TIMEOUT = 60 * 1000; // 1 min, in milliseconds

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

int main(void)
{
	Server *myserv = new Server("myserv",  "pass1234");

	//SETTING UP POLL
	std::vector<char> buf_vec(5000);
	PollManager			pollManager;

	//setup first fd for serverfd, where poll will listen for incoming connections
	pollManager.addFD(myserv->getServerFd());

	int poll_status;
	int stat;
	bool allok = true;
	while(allok)
	{
		poll_status = pollManager.poll(POLL_TIMEOUT);
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
		//otherwise, some event was called on one of the pollManager. check all of them
		for (int i = 0; i < pollManager.getSize(); i++)
		{
			memset(buf_vec.data(), 0 ,5000);

			if(pollManager.getFds()[i].revents == 0) //not the called event
				continue;
			if(pollManager.getFds()[i].revents != POLLIN) //not the correct event
			{
				printf("  Error! revents = %d\n", pollManager[i].revents);
				allok = false;
				break;
			}
			if (pollManager.getFds()[i].fd == myserv->getServerFd()) // called on serverfd
			{
				allok = accept_new_connection(myserv->getServerFd(), pollManager);
				if (!allok)
					break;
			}
			else //called on connection fds
			{
				printf("From Descriptor %d :\n", pollManager[i].fd);
				stat = recv(pollManager.getFds()[i].fd, buf_vec.data(), buf_vec.size(), 0);
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
					close(pollManager[i].fd);
					pollManager[i].fd = -1;
					break;
				}
				else
				{
					std::string msg;
					std::cout	<< strlen(buf_vec.data())
								<< "{"
								<< buf_vec.data()
								<< "}" << std::endl;
					if (strcmp(buf_vec.data(), ("PASS " + myserv->getServerPass()).c_str()) == 0)
					{
						msg  = "user";
						char c = pollManager[i].fd + '0';
						msg.append(&c);
						myserv->addClient(
							msg
							, "msg", pollManager[i].fd, "localmachine");

					msg = myserv->getClients().back()->getUserName() + " registered!";
						stat = send(pollManager[i].fd, "yay!",5, 0);
					}
				}

			}
		}
	}


	return 0;
}