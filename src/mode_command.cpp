
#include "Server.hpp"
//#include "PollManager.hpp"
/*
std::string toLower(std::string str) {
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = std::tolower(str[i]);
	}
	return str;
}*/

static bool isValidMode(std::string str, std::string *unknown) {
	if (str.length() > 4)
		return false;
//	std::cout << str[0] << std::endl;
	if ('+' == str[0] || '-' == str[0]) {
		str.erase(0,1);
		if (str.find_first_not_of("itkol") != str.npos) {
			unknown->append(str, str.find_first_not_of("itkol"), 1);
			return false;
		}
		return true;
	}
	return false;
}

/*
 * 	to test:
 * MODE
 * MODE wrongchannel
 * MODE rightchannel
 * MODE wrongmodes
 * MODE rightmodes
 * MODE wrongchannel wrongmodes
 * MODE wrongchannel rightmodes
 * MODE rightchannel wrongmodes
 * MODE rightchannel rightmodes
 */


/*
	todo

		- check if key can be changes when is already set -> it can be changed
		- if error encountered - should we handle other modes? for now no
		- what will happen with users in the channel in case when new max limit is less than current one, or when 0
		- when oper is added - should regular MODE reply be sent or something else?
			also - should NAMES be sent directly after?
*/
void Server::set_modes(Client &client, Channel &channel, std::vector<std::string> args) {
	std::string str = "MODE " + channel.getName();
	for(int i = 0; i < args.size(); i++)
		str.append(" ").append(args[i]);


	if (!channel.isOper(client)) { // client is not an oper
		serverReply(client, ERR_CHANOPRIVSNEEDED(channel.getName()));
		return;
	}

	std::string mode = args[0];
	args.erase(args.begin(), args.begin() + 1); //only params are in the vector
	std::string invalid = "";
	if (isValidMode(mode, &invalid) == false) { // check for valid modes
		serverReply(client, ERR_UNKNOWNMODE(invalid, channel.getName()));
		return;
	}
	if (channel.getName()[0] == '+') {//	 	 !!!!!!!!!!! channel is with '+' prefix, for that only 't' mode is available
		if (mode == "+t") {
			channel.setTopicFlag('+');
		} else if (mode == "-t") {
			channel.setTopicFlag('-');
		} else {
			serverReply(client, ERR_UNKNOWNMODE(mode, channel.getName()));
		}
		channel.sendToAll(client, str);
		channel.sendToClient(client, str);
		return;
	}
	char sign = mode[0];
	mode.erase(mode.begin());

	for (int j = 0; j < mode.size(); j++) {
		char c = mode[j];
		switch (c) {
			case 't': {
				channel.setTopicFlag(sign);
				break;
				}
			case 'k': {
				if (sign == '-')
					channel.setKey("");
				else {
					if (sign == '+' && !channel.getKey().empty()) { //key is already setted
						serverReply(client, ERR_KEYSET(channel.getName()));
						return;
					}
					if (args.size() == 0) { //not enough params
						serverReply(client, ERR_NEEDMOREPARAMS("MODE"));
						return;
					}
					channel.setKey(args[0]);
					args.erase(args.begin(), args.begin() + 1);
				}
				break;
			}
			case 'i': {
				channel.setInviteOnly(sign);
				break;
			}
			case 'o': {
				if (args.size() == 0) { //not enough params
					serverReply(client, ERR_NEEDMOREPARAMS("MODE"));
					return;
				}
				if (!clientRegistered(args[0]))
				{
					serverReply(client, ERR_NOSUCHNICK(channel.getName()));
					return;
				}
				Client &cl = getClientByNick(args[0]);
				if (!channel.isMember(cl)) { //	in case when client exists but is not a member of channel
					serverReply(client, ERR_USERNOTINCHANNEL(args[0], channel.getName()));
					return;
				}
				if (sign == '+')
					channel.addOper(cl);
				else
					channel.removeOper(cl);
				args.erase(args.begin(), args.begin() + 1);
				break;
			}
			case 'l': {
				if (sign == '-')
				{
					channel.setMaxLim(0);
					break;
				}
				if (args.size() == 0) { //not enough params
					serverReply(client, ERR_NEEDMOREPARAMS("MODE"));
					return;
				}
				try {
					channel.setMaxLim(atol(args[0].c_str()));
					break;
				}
				catch (const std::exception &e) {
					std::cerr << e.what() << std::endl; //should be some message for client???
					return;
				}
				args.erase(args.begin(), args.begin() + 1);
			}
		}
	}
	channel.sendToAll(client, str);
	channel.sendToClient(client, str);
}

void	Server::mode(Client &client, std::vector<std::string> cmd) {
	if (!client.isAuthed()) { //not registered
		serverReply(client, ERR_NOTREGISTERED);
		return;
	}
	if (cmd.size() == 1) { //not enough parameters
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
		return;
	}
			//args[0] - channelname; args[1] - modes; further args[i] - arguments for modes
	std::vector<std::string> args = split(cmd[1], " ");
	std::string channel = args[0];

	if (checkAndLowercaseChannelName(channel) == -1) { // checks actually channel name validity
		serverReply(client, ERR_BADCHANMASK(channel));
		return;
	}
	if (!channelExists(channel)) { // checks channel exists
		serverReply(client, ERR_NOSUCHCHANNEL(channel));
		return;
	}
	if (!getChannelByName(channel).isMember(client)) //check user is on channel
	{
		serverReply(client,ERR_NOTONCHANNEL(channel));
		return;
	}

	Channel &ch = getChannelByName(channel);
	if (args.size() > 1) {
		args.erase(args.begin(), args.begin() + 1); // now there are only arguments for modes, or it's empty
		set_modes(client, ch, args);
	}
	else
		ch.sendToClient(client, RPL_CHANNELMODEIS(client, ch));
}