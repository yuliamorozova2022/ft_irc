#ifndef __INCLUDES__
#define __INCLUDES__

#include <iostream>
#include <map>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <string>
#include <cctype>
#include <climits>


#include "Channel.hpp"
#include "Client.hpp"
#include "PollManager.hpp"
#include "Server.hpp"

class Server;
class Channel;
class Client;
class PollManager;

// ################################################
#define USER_NOT_REGISTERED "User not registered :("
#define ERR_NOTREGISTERED "451 :You have not registered"
#define ERR_ALREADYREGISTRED "462 :You may not reregister"
#define ERR_PASSWDMISMATCH "464 :Password incorrect"
// ################################################


//replies.cpp
std::string RPL_WELCOME (Client &c);
std::string RPL_YOURHOST (Client &c);
std::string RPL_CREATED (Client &c);
std::string RPL_MYINFO (Client &c);
std::string RPL_ENDOFNAMES (Client &c, Channel &ch);
std::string RPL_TOPIC (Client &c, Channel &ch);
std::string RPL_NOTOPIC (std::string c);
std::string RPL_INVITING (Client &c1, Client &c2, Channel &ch);

std::string ERR_INVITEONLYCHAN (std::string c);
std::string ERR_NICKNAMEINUSE (std::string c);
std::string ERR_NEEDMOREPARAMS (std::string c);
std::string ERR_UNKNOWNCOMMAND (std::string c);
std::string ERR_ERRONEUSNICKNAME (std::string c);
std::string ERR_BADCHANMASK (std::string c);
std::string ERR_NORECIPIENT (std::string c);
std::string ERR_NOSUCHNICK (std::string c);
std::string ERR_CHANOPRIVSNEEDED (std::string c);
std::string ERR_NOSUCHCHANNEL (std::string c);
std::string ERR_USERONCHANNEL (Client &c, Channel &ch);
std::string ERR_NOTONCHANNEL (std::string c);
std::string ERR_UNKNOWNMODE (std::string mode, std::string channel);
std::string ERR_USERNOTINCHANNEL (std::string user, std::string channel);


//tools.cpp
std::string					get_command(Client &client, int &stat);
bool						allowed_symbol_check(std::string params);
std::vector<std::string>	split(std::string str, std::string separator);
int							getChannelName(std::string &cname);
std::string					toLower(std::string str);

#endif