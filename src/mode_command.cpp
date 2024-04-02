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

/*	to test:
 * MODE
 * MODE wrongchannel
 * MODE rightchannel
 * MODE wrongmodes
 * MODE rightmodes
 * MODE wrongchannel wrongmodes
 * MODE wrongchannel rightmodes
 * MODE rightchannel wrongmodes
 * MODE rightchannel rightmodes
 * */

std::string ERR_UNKNOWNMODE (std::string c, std::string channel);
std::string ERR_USERNOTINCHANNEL (std::string a, std::string b);

void	Server::mode(Client &client, std::vector<std::string> cmd) {
	if (!client.isAuthed()) { //not registered
		serverReply(client, ERR_NOTREGISTERED);
		return;
	}
	if (cmd.size() == 1) { //not enough parameters
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
		return;
	}
	std::vector<std::string> args = split(cmd[1], " ");
	std::string channel = args[0];
	channel = toLower(channel);
//	std::cout << "converted channel name is " << channel << std::endl;
//	std::cout << "args[0] is " << args[0] << std::endl;
	if ((args.size() < 2) || (args.size() >= 2 && args[1].empty())) { //not enough parameters
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
		return;
	}
		//channel check
		// Channel names should be case insensitive
		// need to check if user is in the channel?
	std::map<std::string, Channel *> tmp = getChannels();
	if (tmp.find(channel) == tmp.end()) {
		serverReply(client, ERR_USERNOTINCHANNEL(client.getNickName(), args[0]));
		return;
	}
	std::string invalid = "";
	if (isValidMode(args[1], &invalid) == false) { // check for valid modes
		if (invalid.empty() == false)
			serverReply(client, ERR_UNKNOWNMODE(invalid, channel));
		else
			serverReply(client, ERR_UNKNOWNMODE(args[1], channel));
		return;
	}

	if (!getChannelByName(channel).isOper(client)) { // client is not an oper
		serverReply(client, ERR_CHANOPRIVSNEEDED(channel));
		return;
	}
//	if () { // channel is with '+' prefix, for that only 't' mode is available
//	}
		// MODE +ik key
	if (args[1][0] == '+') {
		for (int j = 1; j < args[1].size(); j++) {
			char c = args[1][j];
			switch (c) {
				case 't':
					std::cout << c << std::endl;
					break;
				case 'k':
					std::cout << c << std::endl;
					break;
				case 'i':
					std::cout << c << std::endl;
					break;
				case 'o':
					std::cout << c << std::endl;
					break;
				case 'l':
					std::cout << c << std::endl;
					if (args[2].empty()) { // wrong condition
						serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
						return;
					}
					try {
						Channel ch = getChannelByName(channel);
						//what will happen with users in the channel in case when new max limit is less than current one
						ch.setMaxLim(atol(args[2].c_str())); // should be modified for try-catch
						break;
					}
					catch (const std::exception &e) {
						std::cerr << e.what() << std::endl;
						return;
					}
			}
		}
	}
//	} else  {
//		(args[1][0] == '-')
//		return;
//	}

}