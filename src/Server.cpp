#include "Server.hpp"
#define SERVER_NAME "IRC.42.com"


int Server::_get_set_port(const std::string port_s) {
	if (port_s.length() == 0)
		return -1;
	for (int i = 0; i < (int) port_s.length(); i++) {
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

	// Constructors
Server::Server(const std::string port, std::string serverPass) : _name(SERVER_NAME), _serverFd(_get_set_port(port)), _serverPass(serverPass), _welcomeMsg("Welcome!") {
	if (_serverFd == -1)
		throw std::invalid_argument(get_date_time() + ": Port number invalid, must be int between [0; 65535]");

	std::cout << get_date_time() << ": adding server fd..." << "\e[0m" << std::endl;

	_fds.addFD(_serverFd);
	setupCmds();
	// std::cout << "\e[0;33mConstructor called of Server " << _name << "\e[0m" << std::endl;
}

Server::Server(const int port, const std::string serverPass) : _name(SERVER_NAME), _serverFd(_setup_socket(port)), _serverPass(serverPass), _welcomeMsg("Welcome!") {

	_fds.addFD(_serverFd);
	setupCmds();
	// std::cout << "\e[0;33mConstructor called of Server " << _name << "\e[0m" << std::endl;
}

	// Destructor
Server::~Server() {
	for (std::map<int, Client *>::iterator it = _clients.begin();
			it != _clients.end(); it ++)
			{
				std::cout << get_date_time() << " : deleting " << it->first << std::endl;
				delete(it->second);
			}
	for (std::map<std::string, Channel *>::iterator it = _channels.begin();
			it != _channels.end(); it ++)
			{
				std::cout << get_date_time() << " : deleting " << it->first << std::endl;
				delete(it->second);
			}
	// std::cout << "\e[92mDestructor of Server called\e[0m" << std::endl;
}

	// Getters / Setters
std::string Server::getName() const {return _name;}

int Server::getServerFd() const {return _serverFd;}

std::string Server::getServerPass() const {return _serverPass;}

std::string Server::getWelcomeMsg() const {return _welcomeMsg;}

const std::map<int, Client *> &Server::getClients() const {return _clients;}

const std::map<std::string, Channel *> &Server::getChannels() const {return _channels;}

void Server::setWelcomeMsg(std::string welcomeMsg) {_welcomeMsg = welcomeMsg;}

	// Methods
void Server::createChannel(std::string channelName, Client& creator) {
	_channels.insert(std::pair<std::string, Channel *> (channelName, new Channel(channelName, creator)));
}

void Server::createChannel(std::string channelName,std::string key, Client& creator) {
	_channels.insert(std::pair<std::string, Channel *> (channelName, new Channel(channelName,key, creator)));
}

void Server::createClient(std::string userName,std::string nickName, int fd, std::string host) {
	_clients.insert(std::pair<int, Client *> (fd, new Client(userName, nickName, fd, host)));
}
void Server::createClient(int fd, std::string host) {
	_clients.insert(std::pair<int, Client *> (fd, new Client(fd, host)));
}
void Server::removeClient(int fd) {
	Client *client_ptr = _clients.find(fd)->second;
	delete client_ptr;
	_clients.erase(fd);
	_fds.removeFD(fd);

}


int Server::_setup_socket(int port) {
	struct sockaddr_in address;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	// Creating socket file descriptor
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw std::runtime_error(get_date_time() + ": socket() failed");
	int opt = 1;
	// Forcefully attaching socket to the port
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error(get_date_time() + ": setsockopt() failed");
	// Setting server fd to non-blocking
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(get_date_time() + ": fcntl() failed");

	// Forcefully attaching socket to the port specified
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error(get_date_time() + ": bind() failed");
	if (listen(server_fd, 3) < 0)
		throw std::runtime_error(get_date_time() + ": listen() failed");
	return server_fd;
}

std::string find_revent(short revent)
{
	std::string ret = "";
	if (revent & POLLIN)
		ret += "POLLIN";
	if (revent & POLLRDNORM)
		ret += " POLLRDNORM";
	if (revent & POLLRDBAND)
		ret += " POLLRDBAND";
	if (revent & POLLPRI)
		ret += " POLLPRI";
	if (revent & POLLOUT)
		ret += " POLLOUT";
	if (revent & POLLWRNORM)
		ret += " POLLWRNORM";
	if (revent & POLLWRBAND)
		ret += " POLLWRBAND";
	if (revent & POLLERR)
		ret += " POLLERR";
	if (revent & POLLHUP)
		ret += " POLLHUP";
	if (revent & POLLNVAL)
		ret += " POLLNVAL";

	return ret;
}


void Server::launch() {
	int poll_status;
	while (!g_interrupt) {
		poll_status = _fds.poll(POLL_TIMEOUT);
		if (poll_status < 0) {
			if (errno != EINTR) //interrupted system call
				throw std::runtime_error(get_date_time() + ": poll() failed");
		}
		else if (poll_status == 0)
			throw std::runtime_error(get_date_time() + ": poll() timed out");

		for (int i = 0; i < _fds.getSize(); i++) {
			if (!_fds.getFds()[i].revents)				//if event wasnt on this fd
				continue;
			if ((_fds.getFds()[i].revents & POLLIN) == 0)
//				if (_fds.getFds()[i].revents != POLLIN)//if event is not POLLIN
			{
				std::cout << get_date_time() << ": ";
				std::cout << "Unexpected event from [" << _fds.getFds()[i].fd << "]: " << _fds.getFds()[i].revents << std::endl;
				std::cout << "	event: " << find_revent(_fds.getFds()[i].revents) << std::endl;
				std::cout << "	removing client..." << std::endl;
				removeClient(_fds[i].fd);
				continue;
			}
			if (_fds.getFds()[i].fd == _serverFd)		// called on serverfd
				_accept_new_connection();
			else
				_client_request(i);
		}
	}
}

void Server::_accept_new_connection() {

	int new_connection;
	char hostname[2048];
	sockaddr_in s_address = {};
	socklen_t s_size = sizeof(s_address);

	new_connection = accept(_serverFd, reinterpret_cast<sockaddr *>(&s_address), &s_size);

	if (fcntl(new_connection, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(get_date_time() + ": fcntl() failed");

	if (new_connection < 0)
		throw std::runtime_error(get_date_time() + ": accept() for new client failed");
	else {

		std::cout << get_date_time() << ": ";
		std::cout << "	New incoming connection: " << new_connection << std::endl;
		if (getnameinfo((struct sockaddr *) &s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
			throw std::runtime_error(get_date_time() + ": Error while getting hostname on new client.");

		_fds.addFD(new_connection);
		createClient(new_connection, hostname);
	}
}

void Server::_client_request(int i) {
	std::vector<char> buf_vec(5000);
	std::fill(buf_vec.begin(), buf_vec.end(), 0);
	int stat;
	int fd = _fds[i].fd;
	while (clientRegistered(fd)) { //loop is needed to read all info from socket
		std::string msg = get_command(getClientByFd(fd), stat);
		if (msg == "") //exit from loop condition
			return;
		if (stat == 0) {
			std::cout << get_date_time() << ": ";
			std::cout << "  from " << fd
					  << ": "
					  << "Connection closed"
					  << std::endl;
			removeClient(fd);
			/*If, for some other reason, a client connection is closed without  the
		client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs
		on socket), the server is required to fill in the quit  message  with
		some sort  of  message  reflecting the nature of the serverReply event which
		caused it to happen.
			 */
		} else
			execCmd(getClientByFd(fd), msg);
	}

}

bool	Server::clientRegistered(int fd) const {
	if (_clients.find(fd) != _clients.end())
		return (true);
	return (false);
}
bool	Server::clientRegistered(std::string nick) const
{
	 std::map<int, Client *> clients = getClients();

	for (std::map<int, Client *>::iterator it = clients.begin();
		it != clients.end(); it++)
		{
			if (it->second->getNickName() == nick)
				return true;
		}
	return (false);
}
bool	Server::channelExists(std::string channelName) const
{
	if (_channels.find(channelName) != _channels.end())
		return (true);
	return (false);
}

Client	&Server::getClientByFd(int fd) {
/* 	if (_clients.find(fd) == _clients.end())
		return _clients.end(); */
	Client *client_ptr = _clients.find(fd)->second;
	return (*client_ptr);
}

Client	&Server::getClientByNick(std::string nick)
{
	for (std::map<int, Client *>::iterator it = _clients.begin();
		it != _clients.end(); it++)
		{
			if (it->second->getNickName() == nick)
				return (*it->second);
		}
	return *_clients.end()->second;
}
Channel	&Server::getChannelByName(std::string channelName)
{
	return (*_channels.find(channelName)->second);
}

int		Server::serverReply(Client &client, std::string msg)
{
	//maybe replies need to end in \r\n??
	std::string newstr  = ":" + _name + " " + msg + "\n";
	int stat = send(client.getFd(),newstr.c_str(), newstr.length(), 0);
	return stat;
}

void Server::sendToEveryone(std::string msg)
{ //does not append the server/client prefix!
	std::string newstr  =  msg + "\n";
	for (std::map<int, Client *>::const_iterator it = getClients().begin(); it != getClients().end(); it++)
	{
		send(it->second->getFd(), newstr.c_str(), newstr.length(), 0);
	}
}
void Server::sendMsgToUser(Client &sender, std::string recipient, std::string msg)
{
	msg = sender.getPrefix() + msg + "\n";

	for (std::map<int, Client *>::const_iterator it = getClients().begin();
		it != getClients().end(); it++)
		{
			if (it->second->getNickName() == recipient)
			{
				std::cout << get_date_time() << ": ";
				send(it->second->getFd(), msg.c_str(), msg.length(), 0);
				return;
			}
		}
	serverReply(sender, ERR_NOSUCHNICK(recipient));
}
void Server::sendMsgToChannel(Client &sender, std::string channel, std::string msg)
{
	if (checkAndLowercaseChannelName(channel) == -1) //check channel name
	{
		serverReply(sender, ERR_BADCHANMASK(channel));
		return;
	}
	//check if channel exists
	if (!channelExists(channel))
	{
		serverReply(sender, ERR_NOSUCHCHANNEL(channel));
		return;
	}
	if (!getChannelByName(channel).isMember(sender)) //check user is on channel
	{
		serverReply(sender,ERR_NOTONCHANNEL(channel));
		return;
	}

	getChannelByName(channel).sendToAll(sender, msg);
}