#ifndef __CHANNEL__
# define __CHANNEL__

#include "includes.hpp"

class Client;
class Server;

class Channel {
	private:
		std::string				_name;
		std::string				_topic;
		std::string				_key;
		int						_n_online;
		bool					_inv_only;
		char					_t_mode;
		int						_max_lim;
		Client*					_creator;
		std::map<int, Client*>	_members;
		std::map<int, Client*>	_opers;

		Channel();

	public:
			// Constructors
		Channel(std::string name, Client& creator);
		Channel(std::string name, std::string key, Client& creator);
		// Channel(const Channel &copy);
			// Destructor
		~Channel();

			// Operators
		// Channel & operator=(const Channel &assign);

			// Getters / Setters
		std::string						getName() const;
		std::string						getTopic() const;
		std::string						getKey() const;
		const std::map<int, Client*>	&getMembers() const;
		int								getOnline() const;
		int								getMaxLim() const;
		bool							getInviteOnly() const;
		void							setName(std::string new_name);
		void							setTopic(std::string new_topic);
		void							setTopicFlag(char sign);
		bool							getTopicFlag() const;

			// Methods
		void	addMember(Client &client);
		void	removeMember(Client &client);
		void	addOper(Client &client);
		void	removeOper(Client &client);
		void	sendToAll(Client &client, std::string msg);
		void	sendToClient(Client &client, std::string msg);
		bool	isOper(Client &client);
		bool	setMaxLim(long n);
		void	setInviteOnly(char sign);

};

#endif