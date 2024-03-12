#include "Server.hpp"

	// Constructors

//Server::Server(const std::string name, const int serverFd, const std::string serverPass) : _name(name), _serverFd(serverFd), _serverPass(serverPass) {
//	std::cout << "\e[0;33mFields Constructor called of Server\e[0m" << std::endl;
//}

//Server::Server(const std::string name, const int port, const std::string serverPass) : _name(name), _serverPass(serverPass) {
//	_serverFd = setup_socket(port);
////	if (_serverFd == -1)
////		throw std::invalid_argument();
//	std::cout << "\e[0;33mFields Constructor called of Server " << _name << "\e[0m" << std::endl;
//}

Server::Server(const int port, const std::string serverPass) : _name("IRC_42"), _serverFd(setup_socket(port)), _serverPass(serverPass), _welcomeMsg("Welcome!") {
//	if (_serverFd == -1)
//		throw std::invalid_argument();

	std::cout << "server FD: " << _serverFd << "\e[0m" << std::endl;

	_fds.addFD(_serverFd);
	std::cout << "\e[0;33mConstructor called of Server " << _name << "\e[0m" << std::endl;
}

	// Destructor
Server::~Server() {
//	std::cout << "destructo" << std::endl;
	for (int i = 0; i < _channels.size(); i++)
		delete(_channels[i]);
//	std::cout << "channels done" << std::endl;
//	for (int i = 0; i < _fds.getMaxSize(); i++)
//		_fds.removeFD(_clients[i]->getFd());
//	_fds.removeFD(_serverFd);
	for (int i = 0; i < _clients.size(); i++)
		delete(_clients[i]);
	// _fds.~PollManager();
	std::cout << "\e[92mDestructor called of Server\e[0m" << std::endl;
}

	// Getters / Setters
std::string Server::getName() const {return _name;}

int Server::getServerFd() const {return _serverFd;}

std::string Server::getServerPass() const {return _serverPass;}

std::string Server::getWelcomeMsg() const {return _welcomeMsg;}

std::vector<Client *> Server::getClients() const {return _clients;}

std::vector<Channel *> Server::getChannels() const {return _channels;}

void Server::setWelcomeMsg(std::string welcomeMsg) {_welcomeMsg = welcomeMsg;}

	// Methods
void Server::addChannel(Channel *channel) {
	_channels.push_back(channel);
}

void Server::addChannel(std::string channelName, Client& creator) {
	_channels.push_back(new Channel(channelName, creator));
}

void Server::addChannel(std::string channelName,std::string key, Client& creator) {
	_channels.push_back(new Channel(channelName, key, creator));
}

void Server::addClient(Client *client) {
	_clients.push_back(client);
}

void Server::addClient(std::string userName,std::string nickName, int fd, std::string host) {
	_clients.push_back(new Client(userName, nickName, fd, host));
}

int Server::setup_socket(int port) {
	struct sockaddr_in address;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	// Creating socket file descriptor
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw "socket() failed";
	int opt = 1;
	// Forcefully attaching socket to the port
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw "setsockopt() failed";
	// Forcefully attaching socket to the port specified
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw "bind() failed";
	if (listen(server_fd, 3) < 0)
		throw "listen() failed";
//	std::cout << "socket done" << std::endl;
	return server_fd;
}

void Server::launch() {
	std::cout << "hello" << std::endl;
	std::cout << "serverFD: " << _serverFd  << std::endl;
//	int status;
//	while (true) {
//		status = _fds.poll(POLL_TIMEOUT);
//		if (status < 0)
//			throw std::runtime_error("poll() failed");
//
//	}
}