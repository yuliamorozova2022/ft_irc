#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "PollManager.hpp"

#include <csignal>




/*
	NOTE:
		packages myst be aggregated till a new line char is found.
		nc is good for testing, package will be sent without nl
		std::getLine can be used for this (?)




 */



bool g_interrupt = false;

void signal_handler(int const sig)
{
	(void) sig;
	g_interrupt = true;
	std::cout << "\b\b";

}


int main() {
	int argc = 3;
	std::string argv[] = {"./ircserv", "6666", "test_pass"};
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



