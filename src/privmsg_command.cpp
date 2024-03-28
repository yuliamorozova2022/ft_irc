
#include "includes.hpp"


//options from client:
// PRIVMSG <username> :[.....]	=> :client PRIVMSG <username> :[....]
// PRIVMSG <#channel> :[.....]	=> :client PRIVMSG <#channel> :[....]

void Server::sendMsgToUser(Client &sender, std::string recipient, std::string msg)
{
	msg = sender.getPrefix() + "PRIVMSG " + recipient + " :" + msg + "\n";
	for (std::map<int, Client *>::const_iterator it = getClients().begin();
		it != getClients().end(); it++)
		{
			if (it->second->getNickName() == recipient)
			{
				send(it->second->getFd(), msg.c_str(), msg.length(), 0);
				return;
			}
		}
	serverReply(sender, ERR_NOSUCHNICK(recipient));
}
void Server::sendMsgToChannel(Client &sender, std::string channel, std::string msg)
{
	msg = "PRIVMSG " + channel + " :" + msg + "\n";
	if (getChannelName(channel) == -1) //check channel name
	{
		std::cout << "{" << channel << "}" << std::endl;
		serverReply(sender, ERR_BADCHANMASK(channel));
		return;
	}
	//check if channel exists
	if (getChannels().find(channel) == getChannels().end())
	{
		serverReply(sender, ERR_NOSUCHNICK(channel));
		return;
	}
	getChannels().find(channel)->second->sendToAll(sender, msg);
}

void Server::privmsg(Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() < 2)
	{
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
		return;
	}

	//get first value, which is either the channel or client name
	std::string recipient = cmd[1].substr(0, cmd[1].find(" :"));
	if (recipient.empty())
	{
		serverReply(client, ERR_NORECIPIENT(cmd[0]));
		return;
	}
	std::string message = cmd[1].substr(cmd[1].find(" :") + 2);

	if (recipient[0] == '&' || recipient[0] == '#')
		sendMsgToChannel(client, recipient, message);
	else
		sendMsgToUser(client, recipient, message);
}


/*
	Command: JOIN
	Parameters: <channel>{,<channel>} [<key>{,<key>}]
*/
static void greetClientToChannel(Server &server, Channel &channel, Client &client)
{
	std::vector<std::string> s;
	s.push_back(channel.getName());
	s.push_back(channel.getName());
	server.serverReply(client, RPL_TOPIC(client, channel));
	channel.sendToAll(client, "JOIN " + channel.getName());
	channel.sendToClient(client, "JOIN " + channel.getName());
	server.names(client, s) ;

}
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
					greetClientToChannel(*this, *(getChannels().find(channel_names[i])->second), client);
				}
			}
			else
			{
				Server::getChannels().find(channel_names[i])->second->addMember(client);
				greetClientToChannel(*this, *(getChannels().find(channel_names[i])->second), client);
			}
		}
		else //if channel doesn't already exist, needs to be created
		{
			if (i >= keys.size()) //if no key exists for the value, create without key
				Server::createChannel(channel_names[i], client);
			else
				Server::createChannel(channel_names[i], keys[i], client);
			greetClientToChannel(*this, *(getChannels().find(channel_names[i])->second), client);
		}
	}
}