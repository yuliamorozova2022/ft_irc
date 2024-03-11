#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "./includes.hpp"

class Client;

class Channel
{
private:
	std::string _name;
	std::string _topic;
	std::string _key;
	int _n_online;
	Client *_creator;
	std::map<int, Client *> _members;
	std::map<int, Client *> _opers;

	Channel();

public:
	// Constructors
	Channel(std::string name, Client &creator);
	Channel(std::string name, std::string key, Client &creator);
	// Channel(const Channel &copy);
	// Destructor
	~Channel();

	// Operators
	// Channel & operator=(const Channel &assign);

	// Getters / Setters
	std::string getName() const;
	std::string getTopic() const;
	int getOnline() const;
	void setName(std::string new_name);
	void setTopic(std::string new_topic);

	// Methods
	void addMember(Client &client);
	void addMember(Client *client);
	void removeMember(Client &client);
	void removeMember(Client *client);
	void addOper(Client &client);
	void addOper(Client *client);
	void removeOper(Client &client);
	void removeOper(Client *client);
};

#endif