
#include "Server.hpp"

std::vector<std::string> split(std::string str, std::string separator); //from tools.cpp


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
		if ( Server::getChannels().find(cmd[1]) != Server::getChannels().end())
		{
			if (Server::getChannels().find(cmd[1])->second->getKey() != "")
			{
				if (i < keys.size())
				{
					serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
					return;
//
				}
				//key incorrect
				if (Server::getChannels().find(cmd[1])->second->getKey() == keys[i]) {
					Server::getChannels().find(cmd[1])->second->addMember(client);
					client.addChannel(Server::getChannels().find(cmd[1])->second);
				}
			}
			else
			{
				Server::getChannels().find(cmd[1])->second->addMember(client);
				client.addChannel(Server::getChannels().find(cmd[1])->second);

			}

		}
		else
		{
			if (i >= keys.size()) //if no key exists for the value
				Server::createChannel(cmd[1], client);
			else
				Server::createChannel(cmd[1], keys[i], client);
			client.addChannel(Server::getChannels().find(cmd[1])->second);
		}
	}
}