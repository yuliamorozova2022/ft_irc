#include "Server.hpp"



int setup_server_socket()
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
		throw Server::SocketSetupException();

	// Setting up socket
	if (setsockopt(server_fd, SOL_SOCKET,
					SO_REUSEADDR | SO_REUSEPORT, &opt,
					sizeof(opt)))
		throw Server::SocketSetupException();

	// Forcefully attaching socket to the port specified
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0)
		throw Server::SocketSetupException();
	
	//Telling socket to listen for incoming connections
	if (listen(server_fd, 3) < 0)
		throw Server::SocketSetupException();
	return (server_fd);
}

// Constructors

Server::Server(std::string name, std::string serverPass)
	: _name(name), _serverFd(setup_server_socket()), _serverPass(serverPass), _welcomeMsg("Welcome!")
{
	std::cout << "\e[0;33mFields Constructor called of Server\e[0m" << std::endl;
}

// Destructor
Server::~Server()
{
	for (int i = 0; i < _channels.size(); i++)
		delete (_channels[i]);
	for (int i = 0; i < _clients.size(); i++)
		delete (_clients[i]);
	close(_serverFd);
	std::cout << "\e[92mDestructor called of Server\e[0m" << std::endl;
}

// Getters / Setters
std::string Server::getName() const
{
	return _name;
}
int Server::getServerFd() const
{
	return _serverFd;
}
std::string Server::getServerPass() const
{
	return _serverPass;
}
std::string Server::getWelcomeMsg() const
{
	return _welcomeMsg;
}
void Server::setWelcomeMsg(std::string welcomeMsg)
{
	_welcomeMsg = welcomeMsg;
}

std::vector<Client *> Server::getClients() const
{
	return _clients;
}
std::vector<Channel *> Server::getChannels() const
{
	return _channels;
}

// Methods
const Channel &Server::addChannel(Channel *channel)
{
	_channels.push_back(channel);
	return (*channel);
}

const Channel &Server::addChannel(std::string channelName, Client creator)
{
	Channel *newc = new Channel(channelName, creator);
	_channels.push_back(newc);
	return (*newc);
}

const Channel &Server::addChannel(std::string channelName, std::string key, Client creator)
{
	Channel *newc = new Channel(channelName, key,creator);
	_channels.push_back(newc);
	return (*newc);
}

const Client &Server::addClient(Client *client)
{
	_clients.push_back(client);
	return *client;
}

const Client &Server::addClient(std::string userName, std::string nickName, int fd, std::string host)
{
	Client *newClient = new Client(userName, nickName, fd, host);
	_clients.push_back(newClient);
	std::cout << ">> registered " << userName << "! <<" << std::endl;
	return *newClient;
}

// Exceptions

const char *Server::SocketSetupException::what() const throw()
{
	return "Something went wrong with server socket initialization :(";
}