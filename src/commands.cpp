
#include "Server.hpp"


void Server::setupCmds(void) {
	_cmds.insert(std::pair<std::string, func> ("PASS", &Server::pass));
	_cmds.insert(std::pair<std::string, func> ("PRIVMSG", &Server::privmsg));
	_cmds.insert(std::pair<std::string, func> ("NICK", &Server::nick));
	_cmds.insert(std::pair<std::string, func> ("USER", &Server::user));
	_cmds.insert(std::pair<std::string, func> ("QUIT", &Server::quit));
	_cmds.insert(std::pair<std::string, func> ("HELP", &Server::help));
	_cmds.insert(std::pair<std::string, func> ("JOIN", &Server::join));
	_cmds.insert(std::pair<std::string, func> ("PING", &Server::pingpong));
	_cmds.insert(std::pair<std::string, func> ("NAMES", &Server::names));
	_cmds.insert(std::pair<std::string, func> ("TOPIC", &Server::topic));
	_cmds.insert(std::pair<std::string, func> ("MODE", &Server::mode));
	_cmds.insert(std::pair<std::string, func> ("INVITE", &Server::invite));
	_cmds.insert(std::pair<std::string, func> ("LIST", &Server::list));
	_cmds.insert(std::pair<std::string, func> ("KICK", &Server::kick));
	_cmds.insert(std::pair<std::string, func> ("PART", &Server::part));
}

void Server::execCmd(Client &client, std::string args){
	std::vector<std::string> cmd;

	args = args.substr(0, args.find('\r'));

	std::cout << MAGENTA << client.getPrefix() + ": " << args << DEFAULT << std::endl;

	if (args[0] == ':') // if prefix is present
		args = args.substr(args.find_first_of(' ') + 1);

	size_t i = args.find_first_of(' ');
	cmd.push_back(args.substr(0, i));
	// for avoiding duplication of command
	if (i != args.npos) {
		args.erase(0, args.find_first_of(' ') + 1);
		cmd.push_back(args);
	}

	std::map<std::string, Server::func>::iterator it = _cmds.find(cmd[0]);
	if (it != _cmds.end())
		(this->*(it->second))(client, cmd);
	else
		serverReply(client, ERR_UNKNOWNCOMMAND(cmd[0]));
}

void Server::pass(Client &client, std::vector<std::string> cmd) {
	if (client.isAuthed())
		serverReply(client, ERR_ALREADYREGISTRED);
	else if (cmd.size() == 1)
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
	else if (cmd[1] == _serverPass)
		client.setAuth();
	else
		serverReply(client, ERR_PASSWDMISMATCH);
}

void Server::nick(Client &client, std::vector<std::string> cmd) {
	if (!client.isAuthed())
		serverReply(client, ERR_NOTREGISTERED);
	else if (cmd.size() == 1 || (cmd.size() >= 2 && cmd[1] == ""))
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0])); // should be changed to    431 ERR_NONICKNAMEGIVEN ":No nickname given" ???
	else if (allowed_symbol_check(cmd[1]) == false) //added check for symbols in nickname
		serverReply(client, ERR_ERRONEUSNICKNAME(cmd[1]));
	else {
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end() ; it++) {
			if (it->second->getNickName() == cmd[1]){
				serverReply(client, ERR_NICKNAMEINUSE(cmd[0]));
				return;
			}
		}

		if (client.isRegistered()) // if is changing their nickname
			sendToEveryone(client.getPrefix() + "NICK " + cmd[1]);

		client.setNickName(cmd[1]);
		if (client.getUserName() != "" && !client.isRegistered()) {
			welcomeClient(client);
		}
	}
}

void Server::user(Client &client, std::vector<std::string> cmd) {
	if (!client.isAuthed())
		serverReply(client, ERR_NOTREGISTERED);
	if (client.isRegistered())
		return; //IS this right??? maybe should use some RPL?
	else if (cmd.size() == 1 || (cmd.size() >= 2 && cmd[1] == ""))
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
	else {
		client.setUserName(cmd[1].substr(0, cmd[1].find_first_of(' ')));
		if (client.getNickName() != "") {
			welcomeClient(client);

		}
	}
}

void Server::quit(Client &client, std::vector<std::string> cmd) {
	if (!client.isAuthed())
		serverReply(client, ERR_NOTREGISTERED);
	// instead of all users at server message has to be sent for all users in same channels?
	if (cmd.size() < 2)
		cmd[1] = client.getNickName();
	int tmp_fd = client.getFd();
	std::map <std::string, Channel *> tst = client.getJoinedChannels();
	for (std::map<std::string, Channel *>::iterator it = tst.begin();
		it != tst.end();
		it++)
	{
		it->second->sendToAll(client, "QUIT " + cmd[1]);
		it->second->removeMember(client);
	}
	removeClient(tmp_fd);
	std::cout << "  from " << tmp_fd << ": " << "Connection closed" << std::endl;
}

void Server::help(Client &client, std::vector<std::string> cmd) {
	(void) cmd;
	std::string info = "";

	info.append("\n\e[1;32m"); //green color
	info.append("STEP 1: PASS\n");
	info.append("\e[0m"); //reset color
	info.append("\tUse PASS command to establish connection. e.g: PASS [Server Password]\n");
	info.append("\e[1;32m");
	info.append("STEP 2: NICK\n");
	info.append("\e[0m");
	info.append("\tUse NICK command to set a nickname. e.g: NICK yournickname\n");
	info.append("\e[1;32m");
	info.append("STEP 3: USER\n");
	info.append("\e[0m");
	info.append("\tUse USER command to register your username. e.g: USER yourusername\n\n");
	info.append("\e[1;35m"); //purple color
	info.append("AVAILABLE COMMANDS:\n");
	info.append("\e[0m");
	info.append("\tHELP\n\tPASS\n\tNICK\n\tUSER\n\tQUIT\nPRIVMSG\nPING\nTOPIC\nNAMES\nJOIN\nMODE\nLIST\nINVITE\nKICK\nPART\n");
	info.append("\e[0m");
	serverReply(client, info);
}

void Server::pingpong(Client &client, std::vector<std::string> cmd)
{
	serverReply(client, "PONG " +  cmd[1]);
}

/*
	this lists users in the specified channel[s], also runs after JOIN command
	response with two messages
	RPL_NAMREPLY	=> :<pref> RPL_NAMREPLY <client_nick> = #foobar :foobar1 foobar2 foobar3
	RPL_ENDOFNAMES	=>
 */
void Server::names(Client &client, std::vector<std::string> cmd)
{

// :master.ircgod.com 353 nroth = #linux :@idk
//  :IRC.42.com 353 nroth = #test: @nroth



	/*
	:master.ircgod.com 353 nroth = #test :@nroth1 @nroth

		INCORRECT - the @ symbol is not to mark sender, its to mark all opers. i think?
	 */
	getChannelName(cmd[1]);
	if (Server::getChannels().find(cmd[1]) == Server::getChannels().end())
		return;
	Channel &ch = *(Server::getChannels().find(cmd[1])->second);

		std::string userlist = "";

		for (std::map<int, Client *>::const_iterator i = ch.getMembers().begin();
			i != ch.getMembers().end(); i++)
		{
			if (! userlist.empty())
				userlist += " ";
			if (ch.isOper(*i->second))
				userlist += "@";
			userlist += i->second->getNickName();
std::cout << "user: " + userlist << std::endl;
		}
std::cout << "userlist: " + userlist << std::endl;
		std::string rep = "353 " + client.getNickName() + " = " + ch.getName() + " :" + userlist;

		serverReply(client, rep);
		serverReply(client, RPL_ENDOFNAMES(client, ch));
}

/* CLIENT:	[:nuna!nroth@localhost NAMES #test]
SERVER:
	[
		:irc.ircgod.com 353 nroth = #test :@nroth  nroth
		:irc.ircgod.com 366 nroth #test :End of NAMES list
	]
 */

void Server::topic(Client &client, std::vector<std::string> cmd)
{
	std::vector<std::string> args = split(cmd[1], " :");
	std::string channel_s = args[0];

	if (getChannelName(channel_s) == -1) //check channel name
	{
		serverReply(client, ERR_BADCHANMASK(channel_s));
		return;
	}
	//check if channel exists
	if (getChannels().find(channel_s) == getChannels().end())
	{
		serverReply(client, ERR_NOSUCHNICK(channel_s));
		return;
	}

	if (args.size() == 1 && !getChannelByName(channel_s).getTopic().empty()) //only requesting the channel's topic
		serverReply(client, RPL_TOPIC(client, *(getChannels().find(channel_s)->second)));
	else if (args.size() == 1 && getChannelByName(channel_s).getTopic().empty()) //if topic is empty
		serverReply(client, RPL_NOTOPIC(channel_s));
	else //wanting to change the channel's topic
	{

		if (!getChannelByName(channel_s).isMember(client)) //if client is not in channel
		{
			serverReply(client, ERR_NOTONCHANNEL(channel_s));
			return;
		}
		if (!getChannelByName(channel_s).isOper(client) && getChannelByName(channel_s).getTopicFlag()) //if client is not oper
		{
			serverReply(client, ERR_CHANOPRIVSNEEDED(channel_s));
			return;
		}
		getChannelByName(channel_s).setTopic(args[1]);
		getChannelByName(channel_s).sendToAll
			(client, "TOPIC " + channel_s + " :" + getChannelByName(channel_s).getTopic() );
		getChannelByName(channel_s).sendToClient
			(client, "TOPIC " + channel_s + " :" + getChannelByName(channel_s).getTopic() );
	}
}

/*
Angel->server:
INVITE Wiz #Twilight_Zone
	; Command to invite WiZ to #Twilight_zone

Server ->  Wiz:
:Angel!wings@irc.org INVITE Wiz #Dust
	; Message to WiZ when he has been invited by user Angel to channel #Dust
*/
void Server::invite(Client &client, std::vector<std::string> cmd)
{
	std::vector<std::string> splot = split(cmd[1]," ");

	if (splot.size() < 2)
		{serverReply(client, ERR_NEEDMOREPARAMS(cmd[0])); return;}

	if (!clientRegistered(splot[0]))
		{serverReply(client, ERR_NOSUCHNICK(splot[0])); return; }

	if (getChannels().find(splot[1]) == getChannels().end())
		{serverReply(client, ERR_NOSUCHCHANNEL(splot[1])); return; }


	Client &targetClient = getClientByNick(splot[0]);
	Channel &targetChannel = getChannelByName(splot[1]);


	//if client is already in channel => ERR_USERONCHANNEL
	if (targetChannel.isMember(targetClient))
		{serverReply(client, ERR_USERONCHANNEL(targetClient, targetChannel)); return; }

	//if sender is not on channel => ERR_NOTONCHANNEL
	if (!targetChannel.isMember(client))
		{serverReply(client, ERR_NOTONCHANNEL(targetChannel.getName())); return; }

	//if channel is invite only by opers => ERR_CHANOPRIVSNEEDED
	if (targetChannel.getInviteOnly() && !targetChannel.isOper(client))
		{serverReply(client, ERR_CHANOPRIVSNEEDED(targetChannel.getName())); return; }

	//if everything is ok: send Invite, send RPL_INVITING

	targetChannel.invite(targetClient);

	sendMsgToUser(client, targetClient.getNickName(), cmd[0] + " " + splot[0] + " " + splot[1]);
	serverReply(client, RPL_INVITING(client, targetClient, targetChannel));
}


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
	message = "PRIVMSG " + recipient + message;

	if (recipient[0] == '&' || recipient[0] == '#')
		sendMsgToChannel(client, recipient, message);
	else
		sendMsgToUser(client, recipient, message);
}

static void greetClientToChannel(Server &server, Channel &channel, Client &client)
{
	std::vector<std::string> s;
	s.push_back(channel.getName());
	s.push_back(channel.getName());

//	channel.getOnline()++;
	std::cout << channel.getName() << " online: " << channel.getOnline() << std::endl;
	server.serverReply(client, RPL_TOPIC(client, channel));
	channel.sendToAll(client, "JOIN " + channel.getName());
	channel.sendToClient(client, "JOIN " + channel.getName());
	server.names(client, s) ;

}
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
				if (i >= keys.size())
				{
					serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
					continue;
				}

				if (ch->getKey() == keys[i]) {
					ch->addMember(client);
					greetClientToChannel(*this, *(getChannels().find(channel_names[i])->second), client);
				}
				else
					serverReply(client, ERR_BADCHANNELKEY(ch->getName()));
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
}

/*
reply to [/LIST] with one channel:

	:master.ircgod.com 322 nroth #linux :1
	:master.ircgod.com 323 nroth :End of LIST

RPL_LIST: 322 <username> <channelname> :<channelcount>


*/
void Server::list(Client &client, std::vector<std::string> cmd)
{
	std::vector<Channel> channelList;
	std::vector<std::string> channelNames;

	if (cmd.size() == 1)
	{
		for (std::map<std::string, Channel *>::iterator it = _channels.begin();
		it != _channels.end(); it++)
		{
			serverReply(client, RPL_LIST(client, *it->second));
			std::cout << LMAGENTA << RPL_LIST(client, *it->second) << DEFAULT << std::endl;
		}

	}
	else if (cmd.size() == 2)
	{
		channelNames = split(cmd[1], ",");
		for (std::vector<std::string>::iterator it = channelNames.begin();
		it != channelNames.end(); it++)
		{
			if (channelExists(*it))
			{
				serverReply(client, RPL_LIST(client, getChannelByName(*it)));
				std::cout << LMAGENTA << RPL_LIST(client, getChannelByName(*it)) << DEFAULT << std::endl;

			}
			else
				serverReply(client, ERR_NOSUCHCHANNEL(*it));
		}
	}
	serverReply(client, RPL_LISTEND(client));
}

/* :nuna!nuna@localhost : KICK #test anotheruser :i dont like them */
//kick removes client from chanel, but doesnt ban them!
// everyone in channel should get a kick message
void Server::kick(Client &client, std::vector<std::string> cmd)
{
	std::string msg = "";
	if (cmd[1].find(" :") != std::string::npos)
	{
		msg = cmd[1].substr(cmd[1].find(':'));
		cmd[1] = cmd[1].substr(0, cmd[1].find(" :"));
	}




	std::string channelname = toLower(split(cmd[1], " ")[0]);
	std::string clientname = split(cmd[1], " ")[1];
	if (!clientRegistered(clientname))
	{
		serverReply(client, ERR_NOSUCHNICK(clientname));
		return;
	}
	if (!channelExists(channelname))
	{
		serverReply(client, ERR_NOSUCHCHANNEL(channelname));
		return;
	}

	if (!getChannelByName(channelname).isMember(getClientByNick(clientname)))
	{
		serverReply(client, ERR_USERNOTINCHANNEL(clientname, channelname));
		return;
	}
	if (!getChannelByName(channelname).isOper(client))
	{
		serverReply(client, ERR_CHANOPRIVSNEEDED(client.getNickName()));
		return;
	}

	sendMsgToChannel(client, channelname, cmd[0] +" "+ cmd[1] + " " + msg);
	getChannelByName(channelname).removeMember(getClientByNick(clientname));
}



//Command: PART
//   Parameters: <channel> *( "," <channel> ) [ <Part Message> ]
//Numeric Replies:
//           ERR_NEEDMOREPARAMS   ERR_NOSUCHCHANNEL   ERR_NOTONCHANNEL

//:WiZ!jto@tolsun.oulu.fi PART #playzone :I lost
void Server::part(Client &client, std::vector<std::string> cmd) {
	if (cmd.size() < 2) {
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
		return ;
	}
	std::string msg = "";
	if (cmd[1].find(" :") != std::string::npos) {
		msg = cmd[1].substr(cmd[1].find(':'));
		cmd[1] = cmd[1].substr(0, cmd[1].find(" :"));
	}
	std::vector<std::string> channels = split(cmd[1], ",");
	for (int i = 0; i < channels.size(); ++i) {

		std::cout << "message to send from PART is: {" << msg << "}\nchannelname is: " << channels[i]<< std::endl;

		if (channels[i].empty() || !channelExists(toLower(channels[i]))) {
			serverReply(client, ERR_NOSUCHCHANNEL(channels[i]));
			// return;
			continue;
		}
		Channel &tmp = getChannelByName(toLower(channels[i]));
		if (!tmp.isMember(client)) {
			serverReply(client, ERR_NOTONCHANNEL(channels[i]));
			// return;
			continue;
		}

		tmp.sendToAll(client, cmd[0] + " " + tmp.getName() + " " + msg);
		tmp.sendToClient(client, cmd[0] + " " + tmp.getName() + " " + msg);

		tmp.removeMember(client);
	}
}