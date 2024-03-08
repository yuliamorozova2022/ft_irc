#include "Channel.hpp"


// Constructors
	Channel::Channel() {}

	Channel::Channel(std::string name, Client& creator) :_name(name), _creator(creator), _n_online(0) {
		std::cout << "\e[0;33mInt Constructor called for Channel\e[0m" << std::endl;
		_topic = NULL;
		_members = NULL;
		_opers = NULL;
	}

	Channel::Channel(std::string name, std::string key, Client& creator) :_name(name), _key(key); _creator(creator), _n_online(0) {
		std::cout << "\e[0;33mInt Constructor called for private Channel\e[0m" << std::endl;
		_topic = NULL;
		_members = NULL;
		_opers = NULL;
	}

/* 	Channel::Channel(const Channel &copy)
	{
		std::cout << "\e[0;33mCopy Constructor called for Channel\e[0m" << std::endl;
	} */


// Destructor
	Channel::~Channel()
	{
		std::cout << "\e[92mDestructor called for Channel\e[0m" << std::endl;
	}

/* 
// Operators
	Channel & Channel::operator=(const Channel &assign)
	{
		return *this;
	} */


// Getters / Setters
	std::string Channel::getName() const {return _name;}
	std::string Channel::getTopic() const {return _topic;}
	int Channel::getOnline() const {return _n_online;}

	void Channel::setName(std::string new_name) const {_name = new_name;}
	void Channel::setTopic(std::string new_topic) const {_topic = new_topic;}

//methods
	
	