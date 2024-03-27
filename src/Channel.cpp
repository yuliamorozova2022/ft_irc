#include "Channel.hpp"

	// Constructors
Channel::Channel() {}

Channel::Channel(std::string name, Client& creator) :_name(name), _creator(&creator), _n_online(0) {
	std::cout << "\e[0;33mInt Constructor called for Channel\e[0m" << std::endl;
	addOper(creator);
	_topic = "default_topic";
}

Channel::Channel(std::string name, std::string key, Client& creator) :_name(name), _key(key), _creator(&creator), _n_online(0) {
	std::cout << "\e[0;33mInt Constructor called for private Channel\e[0m" << std::endl;
	addOper(creator);
	_topic = "default_topic";
}


	// Destructor
Channel::~Channel() {
	std::cout << "\e[92mDestructor called for Channel " << _name << "\e[0m" << std::endl;
}

	// Getters / Setters
std::string Channel::getKey() const {return _key;}
std::string Channel::getName() const {return _name;}
std::string Channel::getTopic() const {return _topic;}
int Channel::getOnline() const {return _n_online;}
const std::map<int, Client*> &Channel::getMembers() const {return _members;}

void Channel::setName(std::string new_name) {_name = new_name;}
void Channel::setTopic(std::string new_topic) {_topic = new_topic;}

	//methods
void Channel::addMember(Client &client) {
	_members.insert(std::pair<int, Client *> (client.getFd(), &client));
	std::cout << "\e[92mAdding member " << client.getNickName() << " to " << _name << "\e[0m" << std::endl;
	client.addChannel(this);
}

void Channel::removeMember(Client &client) {
	_members.erase(client.getFd());
	std::cout << "\e[92mRemoving member " << client.getNickName() << " from " << _name << "\e[0m" << std::endl;
	client.removeChannel(this);
}

void Channel::addOper(Client &client) {
	_opers.insert(std::pair<int, Client *> ( client.getFd(), &client));
	std::cout << "\e[92mAdding oper " << client.getNickName() << " from " << _name << "\e[0m" << std::endl;

	addMember(client);
}

void Channel::removeOper(Client &client) {
	_opers.erase(client.getFd());
	std::cout << "\e[92mRemoving oper " << client.getNickName() << " from " << _name << "\e[0m" << std::endl;

	removeMember(client);
}

void	Channel::sendToAll(Client &client, std::string msg)
{
	msg = client.getPrefix() + msg + "\n";
	// std::map <int, Client *> tst = _members;
	int stat;
	for (std::map<int, Client *>::iterator it = _members.begin();
		it != _members.end();
		it++)
	{
		// if (it->first != client.getFd())
		// {
			stat = send(it->second->getFd(),msg.c_str(), msg.length(), 0);
			if (stat == -1)
				throw std::runtime_error("sendToAll () failed!");
		// }
	}
}
