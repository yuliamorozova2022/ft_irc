#ifndef __CLIENT__
# define __CLIENT__

#include <iostream>
#include <string>
#include <map>

#include "Channel.hpp"
#include "includes.hpp"

class Channel;

class Client {
	private:
		Client();
		std::string							_userName;
		std::string							_nickName;
		const int							_fd;
		std::string							_host;
		std::map<std::string, Channel *>	_joinedChannels;
		std::string							_msgBuffer;
		bool								_registered; //set as true when both NICK and USER are set
		bool								_auth; //set as true when PASS msg was sent


	public:
			// Constructors
		Client(std::string userName, std::string nickName, int fd, std::string host);
		Client(int fd, std::string host);
			// Destructor
		~Client();
			// Getters
		std::string getUserName() const;
		int getFd() const;
		std::string getNickName() const;
		std::string getHost() const;
		const std::string &getMsgBuffer() const;
		void setMsgBuffer(std::string msg);
		void addMsgBuffer(std::string msg);
		std::map<std::string, Channel *> getJoinedChannels() const;
		bool isRegistered() const;
		bool isAuthed() const;

			// Setters
		void setNickName(std::string nickName);
		void setUserName(std::string nickName);
		void setAuth();
		void setRegistered();
			//methods
		void	addChannel(Channel *channel);
		void	removeChannel(Channel *channel);
		std::string	getPrefix(void);

};

#endif