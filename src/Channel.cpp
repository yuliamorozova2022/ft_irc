#include "Channel.hpp"


	// Constructors
Channel::Channel() {}

Channel::Channel(std::string name, Client& creator) :_name(name), _creator(&creator), _n_online(0) {
	std::cout << "\e[0;33mInt Constructor called for Channel\e[0m" << std::endl;
	_topic = "";
}

Channel::Channel(std::string name, std::string key, Client& creator) :_name(name), _key(key), _creator(&creator), _n_online(0) {
	std::cout << "\e[0;33mInt Constructor called for private Channel\e[0m" << std::endl;
	_topic = "";
}


	// Destructor
Channel::~Channel() {
	std::cout << "\e[92mDestructor called for Channel " << _name << "\e[0m" << std::endl;
}

	// Getters / Setters
std::string Channel::getName() const {return _name;}
std::string Channel::getTopic() const {return _topic;}
int Channel::getOnline() const {return _n_online;}

void Channel::setName(std::string new_name) {_name = new_name;}
void Channel::setTopic(std::string new_topic) {_topic = new_topic;}

	//methods
void Channel::addMember(Client &client) {
	_members.insert(std::pair<int, Client *> (client.getFd(), &client));
}

void Channel::removeMember(Client &client) {
	_members.erase(client.getFd());
}

void Channel::addOper(Client &client) {
	_opers.insert(std::pair<int, Client *> ( client.getFd(), &client));
	_members.insert(
		std::pair<int, Client *>
		( client.getFd(), &client));
}

void Channel::removeOper(Client &client) {
	_opers.erase(client.getFd());
	_members.erase(client.getFd());
}
