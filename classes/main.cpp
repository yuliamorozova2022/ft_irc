#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

//void channel_creator(std::string name, Client &user) {
//}

#include <arpa/inet.h>
#include <sys/socket.h>
#include <cerrno>
//#define PORT = 6666
const int PORT = 6666;

const int setup_socket() {
    int opt = 1;
    int server_fd;
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port specified
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return (server_fd);
}

int main(void) {
	Client *a = new Client("Auser", "Anick", 3, "42");
	Client *b = new Client("Buser", "Bnick", 4, "42");
	Client *c = new Client("Cuser", "Cnick", 5, "42");


//	Channel ch("myUser", a);
	 Channel *ch = new Channel("myUser", *a);

	std::cout << ch->getName() << std::endl;
	a->addChannel(ch);
//
//	std::cout << "yay" << std::endl;
//
	std::map<std::string, Channel *> temp = a->getJoinedChannels();
	std::cout <<
					temp.find("myUser")->first
	<< std::endl;
////--------------------------------------------------------------

//      JUST TO THINK ABOUT!
//      maybe it's better to write Server constructor as:
//          Server(const std::string name, const int port, const std::string serverPass);   // or std::string port
//      in that case socket connection can be established during object creation

    Server sv("hello", setup_socket(), "test");
    sv.addClient(a);
    sv.addClient(c);
    sv.addClient(b);
////    sv.addChannel("myUser", a);
    sv.addChannel(ch);
    ch->setTopic("testing purposes");
    std::vector<Channel *> tmp = sv.getChannels();
    for (int i = 0; i < tmp.size(); i++) {
        std::cout <<
            "name: " << tmp[i]->getName() << ", topic: " << tmp[i]->getTopic()
        << std::endl;
    }
    std::vector<Client *> tmp1 = sv.getClients();
    for (int i = 0; i < tmp1.size(); i++) {
        std::cout <<
                  "name: " << tmp1[i]->getUserName() << ", nick: " << tmp1[i]->getNickName()
                  << std::endl;
    }

//    delete ch;
//    delete a;
//    delete b;
//    delete c;
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