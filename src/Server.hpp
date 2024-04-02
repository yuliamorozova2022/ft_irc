#ifndef __SERVER__
# define __SERVER__

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>

#include "Channel.hpp"
#include "Client.hpp"
#include "PollManager.hpp"
#include "includes.hpp"

extern bool g_interrupt;

class Client;
class Channel;
class PollManager;
class Server;


class Server {
	private:
		typedef void (Server::*func)(Client &client, std::vector<std::string> cmd);
		Server();
		const std::string		_name;
		const int	  			_serverFd;
		const std::string		_serverPass;
		std::string				_welcomeMsg;
		std::map<int, Client *>	_clients;
		std::map<std::string, Channel *>	_channels;
		std::map<std::string, Server::func>	_cmds;
		PollManager				_fds;

		int			_setup_socket(int port);
		int			_get_set_port(const std::string port_s);

		//connection handling
		void	_accept_new_connection();
		void	_client_request(int i);

	public:

			// Constructors
		Server(const int serverFd, const std::string serverPass);
		Server(const std::string port, const std::string serverPass);

			// Destructor
		~Server();

			// Getters / Setters
		std::string							getName() const;
		int									getServerFd() const;
		std::string							getServerPass() const;
		std::string							getWelcomeMsg() const;
		const std::map<int, Client *>				&getClients() const;
		const std::map<std::string, Channel *>	&getChannels() const;
		void								setWelcomeMsg(std::string welcomeMsg);

			// Methods
		void	launch();
		Client	&getClientByFd(int fd);
		Client	&getClientByNick(std::string nick);
		Channel	&getChannelByName(std::string channelName);
		void	createChannel(std::string channelName, Client& creator);
		void	createChannel(std::string channelName,std::string key, Client& creator);
		void	createClient(std::string userName, std::string nickName, int fd, std::string host);
		void	createClient(int fd, std::string host);
		void	removeClient(int fd);
		bool	clientRegistered(int fd) const;
		bool	clientRegistered(std::string nick) const;
		int		serverReply(Client &client, std::string msg);
		void	welcomeClient(Client &c);

		void	sendMsgToChannel(Client &sender, std::string channel, std::string msg);
		void	sendMsgToUser(Client &sender, std::string reciever, std::string msg);

		void sendToEveryone(std::string msg);


			//Command Handling
		void	setupCmds(void);
		void	execCmd(Client &client, std::string args);
		void	pass(Client &client, std::vector<std::string> cmd);
		void	nick(Client &client, std::vector<std::string> cmd);
		void	user(Client &client, std::vector<std::string> cmd);
		void	quit(Client &client, std::vector<std::string> cmd);
		void	help(Client &client, std::vector<std::string> cmd);
		void	join(Client &client, std::vector<std::string> cmd);
		void	privmsg(Client &client, std::vector<std::string> cmd);
		void	pingpong(Client &client, std::vector<std::string> cmd);
		void	names(Client &client, std::vector<std::string> cmd);
		void	topic(Client &client, std::vector<std::string> cmd);
		void	mode(Client &client, std::vector<std::string> cmd);
		void	invite(Client &client, std::vector<std::string> cmd);
};

#endif