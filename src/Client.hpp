#ifndef __CLIENT__
# define __CLIENT__

#include <iostream>
#include <string>
#include <map>

#include "Channel.hpp"

class Channel;

class Client {
	private:
		Client();
		const std::string					_userName;
		std::string							_nickName;
		const int							_fd;
		std::string							_host;
		std::map<std::string, Channel *>	_joinedChannels;

	public:
			// Constructors
		Client(std::string userName, std::string nickName, int fd, std::string host);
			// Destructor
		~Client();
			// Getters
		const std::string getUserName() const;
		const int getFd() const;
		std::string getNickName() const;
		std::string getHost() const;
		std::map<std::string, Channel *> getJoinedChannels() const;

			// Setters
		void setNickName(std::string nickName);
			//methods
		void	addChannel(Channel *channel);
		void	removeChannel(Channel *channel);

};

#endif