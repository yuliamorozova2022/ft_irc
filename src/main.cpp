#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "PollManager.hpp"

#include <csignal>




bool g_interrupt = false;

void signal_handler(int const sig)
{
	(void) sig;
	g_interrupt = true;
	std::cout << "\b\b";

}


int main(int argc, char *argv[])
{
/* 	if (argc == 1)
	{
		argc = 3;
		argv[0] = "./ircserv";
		argv[1] =  "6666";
		argv[2] = "test";
	} */
	if (argc != 3) {
		std::cout << "Usage ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	signal (SIGINT, signal_handler);

	try {
		Server serv(argv[1], argv[2]);
		serv.launch();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}



