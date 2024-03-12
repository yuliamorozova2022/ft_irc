#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "PollManager.hpp"
#include <cstdlib>

static bool getPort(std::string str, int *port) {
    if (str[0] == 0)
        return false;
    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i]) == false) {
            std::cerr << "Input error: port must contain only digits" << std::endl;
            return false;
        }
    }
    int tmp = atoi(str.c_str());
    if (tmp < 0 || tmp > 65535) {
        std::cerr << "Input error: port must be in range [0; 65535]" << std::endl;
        return false;
    }
    *port = tmp;
    return true;
}

int main() {
    int argc = 3;
    std::string argv[] = {"./ircserv", "6666", "test_pass"};
    if (argc != 3) {
        std::cout << "Usage ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    int port;
    if (getPort(argv[1], &port)== false)
        return 1;
//    std::cout << "port is " << port << std::endl;
    try {
        Server serv(port, argv[2]);
        serv.launch();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "test" << std::endl;
	return 0;
}





//old main version is below
/*
 *
 *#include "./Client.hpp"
#include "./Channel.hpp"


void channel_creator(std::string name, Client &user)
{

}
int main(void)
{
	Client a("Auser", "Anick", 3, "42");
	Client b("Buser", "Bnick", 4, "42");
	Client c("Cuser", "Cnick", 5, "42");


	Channel ch("myUser", a);
	// Channel *ch = new Channel("myUser", a);

	std::cout << ch.getName() << std::endl;
	a.addChannel(&ch);

	std::cout << "yay" << std::endl;

	std::map<std::string, Channel *> temp = a.getJoinedChannels();
	std::cout <<
					temp.find("myUser")->first
	<< std::endl;


// delete ch;
	return 0;
}
 */