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
	std::string channel = toLower(args[0]);
	if ((args.size() < 2) || (args.size() >= 2 && args[1].empty())) { //not enough parameters
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
		return;
	}
	std::map<std::string, Channel *> tmp = getChannels();
	if (tmp.find(channel) == tmp.end()) { // checks actually channel name
		serverReply(client, ERR_USERNOTINCHANNEL(client.getNickName(), args[0]));
		return;
	}
	std::string invalid = "";
	if (isValidMode(args[1], &invalid) == false) { // check for valid modes
		if (invalid.empty() == false)
			serverReply(client, ERR_UNKNOWNMODE(args[1][0] + invalid, channel));
		else //maybe no need
			serverReply(client, ERR_UNKNOWNMODE(args[1], channel));
		return;
	}
	Channel &ch = getChannelByName(channel);
	if (!ch.isOper(client)) { // client is not an oper
		serverReply(client, ERR_CHANOPRIVSNEEDED(ch.getName()));
		return;
	}
//	 	 !!!!!!!!!!! channel is with '+' prefix, for that only 't' mode is available
		// MODE +ik key
		std::string mode = args[1];
		args.erase(args.begin(), args.begin() + 2); // now there are only arguments for modes, or it's empty
	if (mode[0] == '+') {
		mode.erase(mode.begin());
		for (int j = 0; j < mode.size(); j++) {
			char c = mode[j];
			switch (c) {
				case 't': {
					ch.setTopicFlag('+');
					break;
				}
				case 'k': {// how weechat will behave in case when key is set, and oper tries to change it to another without -k before
					// case when key is set, and oper tries to change it to another without -k before
					if (!ch.getKey().empty()) { //key is already setted
						serverReply(client, ERR_KEYSET(ch.getName()));
						return;
					}
					if (args.size() >= j || args[j].empty()) { //not enough params
						serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
						return;
					}
					ch.setKey(args[j]);
					break;
				}
				case 'i': {
					// Channel &ch = getChannelByName(channel);
					ch.setInviteOnly('+');
					break;
				}
				case 'o': {
					if (args.size() >= j || args[j].empty()) {
						serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
						return;
					}
					// Channel &ch = getChannelByName(channel);
					Client &cl = getClientByNick(args[j]);
						// ^^^ when client is not on server function returns "empty client object" that means client can't be channel member
					if (!cl.isAuthed()) {
						serverReply(client, ERR_USERNOTINCHANNEL(args[j], ch.getName()));
						return;
					}
					if (!ch.isMember(cl)) { //	in case when client exists but is not a member of channel
						serverReply(client, ERR_USERNOTINCHANNEL(args[j], ch.getName()));
						return;
					}
					ch.addOper(cl);
					break;
				}
				case 'l': {
					if (args.size() >= j || args[j].empty()) {
						serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
						return;
					}
					try {
						// Channel &ch = getChannelByName(channel);
						//what will happen with users in the channel in case when new max limit is less than current one
						// or when value is 0
						ch.setMaxLim(atol(args[j].c_str())); 
						break;
					}
					catch (const std::exception &e) {
						std::cerr << e.what() << std::endl; //should be some message for client???
						return;
					}
				}
				serverReply(client,RPL_CHANNELMODEIS(ch.getName(), ch.getModes()));
			}
		}
	} else {
		mode.erase(mode.begin());
		for (int j = 0; j < mode.size(); j++) {
			char c = mode[j];
			switch (c) {
				case 't': {
					ch.setTopicFlag('-');
					break;
				}
				case 'k': {
						// how weechat will behave in case when key is set, and oper tries to use mode -k with wrong key?
					
					// if (args.size() >= j || args[j].empty()) { //not enough params
					// 	serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
					// 	return;
					// }
					//should we check match of channel key and params?
					ch.setKey("");
					break;
				}
				case 'i': {
					// Channel &ch = getChannelByName(channel);
					ch.setInviteOnly('-');
					break;
				}

				case 'o': {
					if (args.size() >= j || args[j].empty()) {
						serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
						return;
					}
					// Channel &ch = getChannelByName(channel);
					Client &cl = getClientByNick(args[j]);
						// ^^^ when client is not on server function returns "empty client object" that means client can't be channel member
					if (!cl.isAuthed()) {
						serverReply(client, ERR_USERNOTINCHANNEL(args[j], ch.getName()));
						return;
					}
					if (!ch.isMember(cl)) { //	in case when client is not a member of channel
						serverReply(client, ERR_USERNOTINCHANNEL(args[j], ch.getName()));
						return;
					}
					ch.removeOper(cl);
					break;
				}
				case 'l': {
					try {
						// Channel &ch = getChannelByName(channel);
						ch.setMaxLim(0);
						break;
					}
					catch (const std::exception &e) {
						std::cerr << e.what() << std::endl;
						return;
					}
				}
			}
			serverReply(client,RPL_CHANNELMODEIS(ch.getName(), ch.getModes()));
		}
	}

}