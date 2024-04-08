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
#include <sstream>
#include <string>
#include <cctype>
#include <climits>
#include <sstream>

#include <ctime>

#include "Channel.hpp"
#include "Client.hpp"
#include "PollManager.hpp"
#include "Server.hpp"

// COLORS:
#define DEFAULT		"\e[39m"
#define BLACK		"\e[30m"
#define RED			"\e[31m"
#define GREEN		"\e[32m"
#define YELLOW		"\e[33m"
#define BLUE		"\e[34m"
#define MAGENTA		"\e[35m"
#define CYAN		"\e[36m"
#define LGRAY		"\e[37m"
#define DGRAY		"\e[90m"
#define LRED		"\e[91m"
#define LGREEN		"\e[92m"
#define LYELLOW		"\e[93m"
#define LBLUE		"\e[94m"
#define LMAGENTA	"\e[95m"
#define LCYAN		"\e[96m"
#define WHITE		"\e[97m"
////////////////////////////////////////////////////




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
std::string RPL_CHANNELMODEIS (std::string channel, std::string modes);

std::string RPL_CHANNELMODEIS (std::string channel, std::string modes);

std::string RPL_LIST (Client &c, Channel &ch);
std::string RPL_LISTEND (Client &c);

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
std::string ERR_KEYSET (std::string c);


//tools.cpp
std::string					get_command(Client &client, int &stat);
bool						allowed_symbol_check(std::string params);
std::vector<std::string>	split(std::string str, std::string separator);
int							getChannelName(std::string &cname);
std::string					toLower(std::string str);
std::string                 numb_to_str(long n);
std::string 				get_date_time();

#endif