#include "Server.hpp"


#define SERVER_NAME "IRC_42"
	// Constructors

int Server::_get_set_port(const std::string port_s)
{
	if (port_s.length() == 0)
		return -1;
	for (int i = 0; i < port_s.length(); i++) {
		if (isdigit(port_s[i]) == false) {
			return -1;
		}
	}
	int portFd = atoi(port_s.c_str());
	if (portFd < 0 || portFd > 65535) {
		return -1;
	}
	return _setup_socket(portFd);
}

Server::Server(const std::string port, const std::string serverPass) : _name(SERVER_NAME), _serverFd(_get_set_port(port)), _serverPass(serverPass), _welcomeMsg("Welcome!") {
	if (_serverFd == -1)
		throw std::invalid_argument("Port number invalid, must be int between [0; 65535]");

	std::cout << "adding server fd..." << "\e[0m" << std::endl;

	_fds.addFD(_serverFd);
	std::cout << "\e[0;33mConstructor called of Server " << _name << "\e[0m" << std::endl;
}

Server::Server(const int port, const std::string serverPass) : _name(SERVER_NAME), _serverFd(_setup_socket(port)), _serverPass(serverPass), _welcomeMsg("Welcome!") {

	_fds.addFD(_serverFd);
	std::cout << "\e[0;33mConstructor called of Server " << _name << "\e[0m" << std::endl;
}

	// Destructor
Server::~Server() {
	for (int i = 0; i < _channels.size(); i++)
		delete(_channels[i]);
	for (int i = 0; i < _clients.size(); i++)
		delete(_clients[i]);
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

int Server::_setup_socket(int port) {
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

void Server::launch()
{
	int poll_status;
	while (!g_interrupt)
	{
		poll_status = _fds.poll(POLL_TIMEOUT);
		if (poll_status < 0)
		{
			if (errno != EINTR) //interrupted system call
				throw std::runtime_error("  poll() failed");
		}
		else if (poll_status == 0)
			throw std::runtime_error("  poll() timed out");

		for (int i = 0; i < _fds.getSize(); i++)
		{
			if (_fds.getFds()[i].revents != POLLIN)
				continue;
			if (_fds.getFds()[i].fd == _serverFd) // called on serverfd
				_accept_new_connection();
			else
				_client_request(i);
		}
	}
}

void Server::_accept_new_connection()
{
	int new_connection;
	new_connection = accept(_serverFd, NULL, NULL);
	if (new_connection < 0)
		throw std::runtime_error("  accept() failed");
	else
	{
		std::cout << "  New incoming connection " << new_connection << std::endl;
		_fds.addFD(new_connection);
	}
}

void Server::_client_request(int i)
{
	std::vector<char> buf_vec(5000);
	int stat = recv(_fds[i].fd, buf_vec.data(), buf_vec.size(), 0);
	if (stat < 0)
	{
		// if (errno != EWOULDBLOCK)
		// {
			perror("  recv() failed");
		// }
		return;
	}
	else if (stat == 0)
	{
		std::cout << "  from " << _fds[i].fd
		<< ": "
		<< "Connection closed"
		<< std::endl;
		_fds.removeFD(_fds[i].fd);
	}
	else
	{
		std::cout << "  from " << _fds[i].fd
		<< ": "
		<< buf_vec.data()
		<< std::endl;

	}
}