#ifndef SERVER_HPP
#define SERVER_HPP

#include "./includes.hpp"

class Client;
class Channel;

class Server
{
public:
	// Constructors
	Server(std::string name,
			std::string serverPass);

	// Destructor
	~Server();

	// Getters / Setters
	std::string getName() const;
	int getServerFd() const;
	std::string getServerPass() const;
	std::string getWelcomeMsg() const;
	std::vector<Client *> getClients() const;
	std::vector<Channel *> getChannels() const;

	void setWelcomeMsg(std::string welcomeMsg);

	// Methods
	const Channel &addChannel(Channel *channel);
	const Channel &addChannel(std::string channelName, Client creator);
	const Channel &addChannel(std::string channelName, std::string key, Client creator);

	const Client &addClient(Client *client);
	const Client &addClient(std::string userName, std::string nickName, int fd, std::string host);


	class SocketSetupException: public std::exception
	{ virtual const char* what() const throw(); };
private:
	Server();
	Server(const Server &copy);
	const std::string _name;
	const int _serverFd;
	const std::string _serverPass;
	std::string _welcomeMsg;
	std::vector<Client *> _clients;
	std::vector<Channel *> _channels;
};

#endif