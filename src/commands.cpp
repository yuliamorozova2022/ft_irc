
#include "Server.hpp"

std::string ERR_NEEDMOREPARAMS (std::string c) {return ("461 " + c + " :Not enough parameters");}
std::string ERR_UNKNOWNCOMMAND (std::string c) {return ("421 " + c + " :Unknown command");}

#define ERR_ALREADYREGISTRED "462 :You may not reregister"

void Server::setupCmds(void)
{
	_cmds.insert(std::pair<std::string, func> ("PASS", &Server::pass));
	// _cmds["func1"] = &Server::func1;

}

void	Server::execCmd(Client &client, std::string args)
{
	std::string cmd = args.substr(0, args.find_first_of(" "));

// std::map<std::string, func>::iterator it = _cmds.find(cmd);
std::map<std::string, Server::func>::iterator it = _cmds.find(cmd);
	if (it != _cmds.end())
		(this->*(it->second))(client, args);
	else
		serverReply(client, ERR_UNKNOWNCOMMAND(cmd));
}



void Server::pass (Client &client, std::string args)
{
	if (client.isAuthed())
		serverReply(client, ERR_ALREADYREGISTRED);
	else
	{
		serverReply(client, ERR_NEEDMOREPARAMS(args));
	}
}