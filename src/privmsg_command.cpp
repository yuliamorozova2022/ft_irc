
#include "Server.hpp"


//from tools.cpp
std::vector<std::string> split(std::string str, std::string separator);
int getChannelName(std::string &cname);


void Server::privmsg(Client &client, std::vector<std::string> cmd)
{

}

//options from client:
// PRIVMSG <username> :[.....]	=> :client PRIVMSG <username> :[....]
// PRIVMSG <#chcannel> :[.....]	=> :client PRIVMSG <#channel> :[....]

/*
	Command: JOIN
	Parameters: <channel>{,<channel>} [<key>{,<key>}]
*/
void Server::join(Client &client, std::vector<std::string> cmd)
{

	if (cmd.size() < 2)
	{
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
		return ;
	}

	//separate channel, key list
	if (cmd[1].find(" ") != cmd[1].npos)
	{
		cmd.push_back(cmd[1].substr(cmd[1].find(" ") + 1));
		cmd[1] = cmd[1].substr(0, cmd[1].find(" "));
	}
	else
		cmd.push_back("");

	//separate each list by commas.
	std::vector<std::string> channel_names = split(cmd[1], ",");
	std::vector<std::string> keys = split(cmd[2], ",");

	for (int i = 0; i < channel_names.size(); i++)
	{
		if (getChannelName(channel_names[i]) == -1)
		{
			serverReply(client, ERR_BADCHANMASK(channel_names[i]));
			continue;
		}
		if ( Server::getChannels().find(channel_names[i]) != Server::getChannels().end())
		{
			if (Server::getChannels().find(channel_names[i])->second->getKey() != "")
			{
				if (i < keys.size())
				{
					serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
					return;
				}
				//key incorrect
				if (Server::getChannels().find(channel_names[i])->second->getKey() == keys[i]) {
					Server::getChannels().find(channel_names[i])->second->addMember(client);
					getChannels().find(channel_names[i])->second->sendToAll(client, "JOIN " + _name);
					serverReply(client, RPL_TOPIC(*getChannels().find(channel_names[i])->second));

					/*//A WAY TO CALL NAME REPLY. NOT GOOD BC TOO LONG
 					std::vector<std::string> s;
					s.push_back(channel_names[i]);
					s.push_back(channel_names[i]);
					Server::names(client, s) ;*/
				}
			}
			else
			{
				Server::getChannels().find(channel_names[i])->second->addMember(client);
				getChannels().find(channel_names[i])->second->sendToAll(client, "JOIN " + _name);
				serverReply(client, RPL_TOPIC(*getChannels().find(channel_names[i])->second));
			}
		}
		else //if channel doesn't already exist, needs to be created
		{
			if (i >= keys.size()) //if no key exists for the value, create without key
				Server::createChannel(channel_names[i], client);
			else
				Server::createChannel(channel_names[i], keys[i], client);
			getChannels().find(channel_names[i])->second->sendToAll(client, "JOIN " + _name);
			serverReply(client, RPL_TOPIC(*getChannels().find(channel_names[i])->second));
		}
	}
}