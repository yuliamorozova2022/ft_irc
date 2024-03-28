#include "Server.hpp"
//#include "PollManager.hpp"

void	Server::mode(Client &client, std::vector<std::string> cmd) {
	std::vector<std::string> args = split(cmd[1], " ");
	std::string channel = args[0];
	if (!getChannelByName(channel).isOper(client)) {
		serverReply(client, ERR_CHANOPRIVSNEEDED(channel));
		return;
	}
}