
#include "includes.hpp"


//options from client:
// PRIVMSG <username> :[.....]	=> :client PRIVMSG <username> :[....]
// PRIVMSG <#channel> :[.....]	=> :client PRIVMSG <#channel> :[....]



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
	std::string message = cmd[1].substr(cmd[1].find(" :"));
	message = "PRIVMSG " + recipient + message + "\n";

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

	channel.getOnline()++;
	std::cout << channel.getName() << " online: " << channel.getOnline() << std::endl;
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

	Channel *ch;

	for (size_t i = 0; i < channel_names.size(); i++)
	{
		channel_names[i] = toLower(channel_names[i]);
		if (getChannelName(channel_names[i]) == -1)
		{
			serverReply(client, ERR_BADCHANMASK(channel_names[i]));
			continue;
		}
		if ( Server::getChannels().find(channel_names[i]) != Server::getChannels().end())
		{
			ch = Server::getChannels().find(channel_names[i])->second;
			//check if channel is invite only, and if user is on invited list
			if (ch->getInviteOnly() == true && ch->isInvited(client) == false)
			{
				serverReply(client, ERR_INVITEONLYCHAN(ch->getName()));
				continue;
			}
			if (ch->getKey() != "")
			{
				if (i < keys.size())
				{
					serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
					continue;
				}

				if (ch->getKey() == keys[i]) {
					ch->addMember(client);
					greetClientToChannel(*this, *(getChannels().find(channel_names[i])->second), client);
				}
			}
			else
			{
				ch->addMember(client);
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

	std::cout << channel.getName() << " online: " << channel.getOnline() << std::endl;

}