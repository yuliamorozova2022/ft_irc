#include "Client.hpp"


// Constructor
Client::Client(std::string userName, std::string nickName, int fd, std::string host): _fd(fd){
	_nickName = nickName;
	_userName = userName;
	_host = host;
	_registered = false;
	_auth = false;
	_msgBuffer = "";

	std::cout << "\e[0;33mFields Constructor called of Client\e[0m" << std::endl;
}
Client::Client(int fd, std::string host): _fd(fd) {
	_nickName = "";
	_userName = "";
	_host = host;
	_registered = false;
	_auth = false;
	_msgBuffer = "";

	std::cout << "\e[0;33mFields Constructor called of Client\e[0m" << std::endl;
}

	// Destructor
Client::~Client() {
	std::cout << "\e[92mDestructor called of Client " << _userName <<"\e[0m" << std::endl;
}

	// Getters
std::string Client::getUserName() const					{ return _userName;}
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
void Client::setAuth() { _auth = true;	std::cout << "\e[0;33muser " + _nickName + " authenticated\e[0m" << std::endl;}
void Client::setRegistered() { _registered = true;	std::cout << "\e[0;33muser " + _nickName + " registered\e[0m" << std::endl;}

	//methods
void	Client::addChannel(Channel *channel) {
	_joinedChannels.insert(std::pair<std::string, Channel *>( channel->getName(), channel ));
	std::cout << "\e[92mAdding channel " << channel->getName() << " to " << _nickName << "\e[0m" << std::endl;
}

void	Client::removeChannel(Channel *channel) {
	_joinedChannels.erase(channel->getName());
	std::cout << "\e[92mRemoving channel " << channel->getName() << " from " << _nickName << "\e[0m" << std::endl;
}

const std::string &Client::getMsgBuffer() const {return _msgBuffer;}
void Client::setMsgBuffer(std::string msg) {_msgBuffer = msg;}
void Client::addMsgBuffer(std::string msg){_msgBuffer += msg;}

std::string	Client::getPrefix(void)
{
	// [:nick!user@host ]
	return (":" + _nickName + "!" + _userName + "@" + _host + " ");
}
