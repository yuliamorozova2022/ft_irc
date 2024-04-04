
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
}

void Server::execCmd(Client &client, std::string args){
	std::vector<std::string> cmd;

	args = args.substr(0, args.find('\r'));

	std::cout << client.getPrefix() + ": " << args << std::endl;

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
	info.append("\tHELP\n\tPASS\n\tNICK\n\tUSER\n\tQUIT\nPRIVMSG\nPING\nTOPIC\nNAMES\nJOIN\n");
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
	else //wanting to chage the channel's topic
	{
		if (!getChannelByName(channel_s).isOper(client)) //if client is not oper
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

/*
Angel: 
INVITE Wiz #Twilight_Zone
	; Command to invite WiZ to #Twilight_zone

Server ->  Wiz:
:Angel!wings@irc.org INVITE Wiz #Dust
	; Message to WiZ when he has been invited by user Angel to channel #Dust


*/