
#include "Server.hpp"


std::string ERR_NICKNAMEINUSE (std::string c) {return ("433 " + c + " :Nickname is already in use");}
std::string ERR_NEEDMOREPARAMS (std::string c) {return ("461 " + c + " :Not enough parameters");}
std::string ERR_UNKNOWNCOMMAND (std::string c) {return ("421 " + c + " :Unknown command");}

#define ERR_NOTREGISTERED "451 :You have not registered"
#define ERR_ALREADYREGISTRED "462 :You may not reregister"
#define ERR_PASSWDMISMATCH "464 :Password incorrect"

void Server::setupCmds(void)
{
	_cmds.insert(std::pair<std::string, func> ("PASS", &Server::pass));
	// _cmds["func1"] = &Server::func1;

}

void	Server::execCmd(Client &client, std::string args)
{
	std::vector<std::string> cmd;
	cmd.push_back(args.substr(0, args.find_first_of(" ")));
	cmd.push_back(args.substr(args.find_first_of(" ") + 1));

// std::map<std::string, func>::iterator it = _cmds.find(cmd);
std::map<std::string, Server::func>::iterator it = _cmds.find(cmd[0]);
	if (it != _cmds.end())
		(this->*(it->second))(client, cmd);
	else
		serverReply(client, ERR_UNKNOWNCOMMAND(cmd[0]));
}



void Server::pass (Client &client, std::vector<std::string> cmd)
{
	if (client.isAuthed())
		serverReply(client, ERR_ALREADYREGISTRED);
	else if (cmd.size() == 1)
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
	else if (cmd[1] == _serverPass)
		client.setAuth();
	else
		serverReply(client, ERR_PASSWDMISMATCH);
}

void Server::nick (Client &client, std::vector<std::string> cmd)
{
	if (!client.isAuthed())
		serverReply(client, ERR_NOTREGISTERED);
	else if (cmd.size() == 1)
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
	else
	{
		for (std::map<int, Client *>::iterator it = _clients.begin();
			it != _clients.end() ;
			it++)
		{
			if (it->second->getNickName() == cmd[1]){
				serverReply(client, ERR_NICKNAMEINUSE(cmd[0]));
				return;
				}
		}
		client.setNickName(cmd[1]);
	}
	/* else if (cmd[1] == _serverPass)
		client.setAuth();
	else
		serverReply(client, ERR_PASSWDMISMATCH); */
}