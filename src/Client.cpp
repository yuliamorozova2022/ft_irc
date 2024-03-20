#include "Client.hpp"


// Constructor
Client::Client(std::string userName, std::string nickName, int fd, std::string host): _fd(fd){
	_nickName = nickName;
	_userName = userName;
	_host = host;
	_registered = false;
	_auth = false;
	std::cout << "\e[0;33mFields Constructor called of Client\e[0m" << std::endl;
}
Client::Client(int fd, std::string host): _fd(fd) {
	_nickName = "";
	_userName = "";
	_host = host;
	_registered = false;
	_auth = false;
	std::cout << "\e[0;33mFields Constructor called of Client\e[0m" << std::endl;
}

	// Destructor
Client::~Client() {
	std::cout << "\e[92mDestructor called of Client " << _userName <<"\e[0m" << std::endl;
}

	// Getters
const std::string Client::getUserName() const					{ return _userName;}
std::string Client::getNickName() const						{ return _nickName;}
const int Client::getFd() const								{ return _fd;}
std::string Client::getHost() const							{ return _host;}
std::map<std::string, Channel *> Client::getJoinedChannels() const	{ return _joinedChannels;}
bool  Client::isRegistered() const{ return _registered;}
bool  Client::isAuthed() const{ return _auth;}

	//Setters
void Client::setNickName(std::string nickName) { _nickName = nickName;}
void Client::setUserName(std::string userName) {
	if (_userName == "")
		_userName = userName;
};
void Client::setAuth() { _auth = true;}
void Client::setRegistered() { _registered = true;}

	//methods
void	Client::addChannel(Channel *channel) {
	for (std::map<std::string, Channel *>::iterator i = _joinedChannels.begin(); i != _joinedChannels.end(); i++) {
		if (channel->getName() == i->second->getName())
			return;
	}
	_joinedChannels.insert(std::pair<std::string, Channel *>( channel->getName(), channel ));
}

void	Client::removeChannel(Channel *channel) {
	for (std::map<std::string, Channel *>::iterator i = _joinedChannels.begin(); i != _joinedChannels.end(); i++) {
		if (channel->getName() == i->second->getName())
			_joinedChannels.erase(channel->getName());
	}
}

const std::string &Client::getMsgBuffer() const {return _msgBuffer;}
void Client::setMsgBuffer(std::string msg) {_msgBuffer = msg;}
void Client::addMsgBuffer(std::string msg){_msgBuffer += msg;}

std::string	Client::getPrefix(void)
{
	// [:nick!user@host ]
	return (":" + _nickName + "!" + _userName + "@" + _host + " ");
}
