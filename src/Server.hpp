#ifndef __SERVER__
# define __SERVER__

#include <iostream>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <exception>

#include "Channel.hpp"
#include "Client.hpp"
#include "PollManager.hpp"

class Client;
class Channel;
class PollManager;

class Server {
    private:
        Server();
//      Server(const Server &copy);
        const std::string		_name;
        const int      			_serverFd;
        const std::string		_serverPass;
        std::string				_welcomeMsg;
        std::vector<Client *>	_clients;
        std::vector<Channel *>	_channels;

        PollManager             _fds;
        int setup_socket(int port);

	public:
		    // Constructors
        Server(const int serverFd, const std::string serverPass);
        // Server(const std::string name, const int serverFd, const std::string serverPass);
//      Server(const std::string name, const std::string port, const std::string serverPass);

		    // Destructor
		~Server();

		    // Getters / Setters
		std::string				getName() const;
		int						getServerFd() const;
		std::string				getServerPass() const;
		std::string				getWelcomeMsg() const;
		std::vector<Client *>	getClients() const;
		std::vector<Channel *>	getChannels() const;
		void					setWelcomeMsg(std::string welcomeMsg); // it has to be part of server or just function??
		
		// Methods
        void launch();

		void	addChannel(Channel *channel);
		void	addChannel(std::string channelName, Client& creator);
		void	addChannel(std::string channelName,std::string key, Client& creator);
		
		void	addClient(Client *client);
		void	addClient(std::string userName, std::string nickName, int fd, std::string host);

};

#endif