
#include "Server.hpp"


std::string ERR_NICKNAMEINUSE (std::string c) {return ("433 " + c + " :Nickname is already in use");}
std::string ERR_NEEDMOREPARAMS (std::string c) {return ("461 " + c + " :Not enough parameters");}
std::string ERR_UNKNOWNCOMMAND (std::string c) {return ("421 " + c + " :Unknown command");}
std::string ERR_ERRONEUSNICKNAME (std::string c) {return ("432 " + c + " :Erroneus nickname");}

#define ERR_NOTREGISTERED "451 :You have not registered"
#define ERR_ALREADYREGISTRED "462 :You may not reregister"
#define ERR_PASSWDMISMATCH "464 :Password incorrect"

void Server::setupCmds(void) {
	_cmds.insert(std::pair<std::string, func> ("PASS", &Server::pass));
	_cmds.insert(std::pair<std::string, func> ("NICK", &Server::nick));
	_cmds.insert(std::pair<std::string, func> ("USER", &Server::user));
	_cmds.insert(std::pair<std::string, func> ("QUIT", &Server::quit));
	_cmds.insert(std::pair<std::string, func> ("HELP", &Server::help));
	// _cmds["func1"] = &Server::func1;
}

void Server::execCmd(Client &client, std::string args){
	std::vector<std::string> cmd;

	size_t i = args.find_first_of(' ');
	cmd.push_back(args.substr(0, i));
	// for avoiding duplication of command
	if (i != args.npos) {
	args.erase(0, args.find_first_of(' ') + 1);
	cmd.push_back(args);
	}
//	cmd.push_back(args.substr(0, args.find_first_of(" ")));
//	cmd.push_back(args.substr(args.find_first_of(" ") + 1));

// std::map<std::string, func>::iterator it = _cmds.find(cmd);
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
		client.setNickName(cmd[1]);
		if (client.getUserName() != "") {
			client.setRegistered();
			serverReply(client, client.getNickName() + ", " + getWelcomeMsg());
		}
	}
}

void Server::user(Client &client, std::vector<std::string> cmd) {
	if (!client.isAuthed())
		serverReply(client, ERR_NOTREGISTERED);
	else if (cmd.size() == 1 || (cmd.size() >= 2 && cmd[1] == ""))
		serverReply(client, ERR_NEEDMOREPARAMS(cmd[0]));
	else {
		client.setUserName(cmd[1].substr(0, cmd[1].find_first_of(' ')));
		if (client.getNickName() != "") {
			client.setRegistered();
			serverReply(client, client.getNickName() + ", " + getWelcomeMsg()); // client has to be formatted as:   :nickname!username@hostname  ???
		}
	}
}

void Server::quit(Client &client, std::vector<std::string> cmd) {
	if (!client.isAuthed())
		serverReply(client, ERR_NOTREGISTERED);
	// instead of all users at server message has to be sent for all users in same channels?
	if (cmd.size() >= 2)
		serverReply(client, cmd[1]);
	int tmp_fd = client.getFd();
	removeClient(tmp_fd);
//	close(client.getFd());
//	_clients.erase(tmp_fd);
	_fds.removeFD(tmp_fd);
	std::cout << "  from " << tmp_fd << ": " << "Connection closed" << std::endl;
}

void Server::help(Client &client, std::vector<std::string> cmd) {
//	void (cmd);
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
	info.append("\tHELP\n\tPASS\n\tNICK\n\tUSER\n\tQUIT\n");
	info.append("\e[0m");
	serverReply(client, info);
}
