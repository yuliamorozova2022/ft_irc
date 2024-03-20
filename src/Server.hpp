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

// ################################################
#define USER_NOT_REGISTERED "User not registered :("
#define ERR_NOTREGISTERED "451 :You have not registered"
#define ERR_ALREADYREGISTRED "462 :You may not reregister"
#define ERR_PASSWDMISMATCH "464 :Password incorrect"

// ################################################


inline std::string RPL_WELCOME (Client &c) {return ("001 " + c.getNickName() + " :Welcome to our amazzzzing server! " + c.getNickName());}
inline std::string RPL_YOURHOST (Client &c) {return ("002 " + c.getNickName() + " :your host is IRC42");}
inline std::string RPL_CREATED (Client &c) {return ("003 " + c.getNickName() + " :This server is not yet created");}
inline std::string RPL_MYINFO (Client &c) {return ("004 " + c.getNickName() + " :IRC42");}

inline std::string ERR_NICKNAMEINUSE (std::string c) {return ("433 " + c + " :Nickname is already in use");}
inline std::string ERR_NEEDMOREPARAMS (std::string c) {return ("461 " + c + " :Not enough parameters");}
inline std::string ERR_UNKNOWNCOMMAND (std::string c) {return ("421 " + c + " :Unknown command");}
inline std::string ERR_ERRONEUSNICKNAME (std::string c) {return ("432 " + c + " :Erroneus nickname");}

inline std::string ERR_BADCHANMASK (std::string c) {return ("476 " + c + " :Bad channel mask");}


std::string get_command(Client &client, int &stat);
bool allowed_symbol_check(std::string params);

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
		// void	addChannel(Channel *channel);
		void	createChannel(std::string channelName, Client& creator);
		void	createChannel(std::string channelName,std::string key, Client& creator);
		// void	addClient(Client *client);
		void	createClient(std::string userName, std::string nickName, int fd, std::string host);
		void	createClient(int fd, std::string host);
		void	removeClient(int fd);
		bool	clientRegistered(int fd) const;
		int		serverReply(Client &client, std::string msg);
		void	sendMsgOnChannel(Channel &channel, Client &sender, std::string msg);
		void	welcomeClient(Client &c);


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

};

#endif