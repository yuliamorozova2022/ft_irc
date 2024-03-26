#include "includes.hpp"

// ################################################
#define USER_NOT_REGISTERED "User not registered :("
#define ERR_NOTREGISTERED "451 :You have not registered"
#define ERR_ALREADYREGISTRED "462 :You may not reregister"
#define ERR_PASSWDMISMATCH "464 :Password incorrect"

// ################################################


std::string RPL_WELCOME (Client &c) {return ("001 " + c.getNickName() + " :Welcome to our amazzzzing server! " + c.getNickName());}
std::string RPL_YOURHOST (Client &c) {return ("002 " + c.getNickName() + " :your host is IRC42");}
std::string RPL_CREATED (Client &c) {return ("003 " + c.getNickName() + " :This server is not yet created");}
std::string RPL_MYINFO (Client &c) {return ("004 " + c.getNickName() + " :IRC42");}
std::string RPL_ENDOFNAMES (Client &c, Channel &ch) {
	return ("366 " + c.getNickName() + " " + ch.getName() + " :End of NAMES list");
	}

std::string RPL_TOPIC (Channel &c) {return ("332 " + c.getName() + " :" + c.getTopic());}
// std::string RPL_TOPIC (Channel &c) {return ("332 " + c.getChannelName() + " :" + c.getTopic());}

std::string ERR_NORECIPIENT (std::string c) {return ("411 :No recipient given (" + c + ")");}
std::string ERR_NOSUCHNICK (std::string c) {return ("401 " + c + " :No such nick/channel");}
std::string ERR_NICKNAMEINUSE (std::string c) {return ("433 " + c + " :Nickname is already in use");}
std::string ERR_NEEDMOREPARAMS (std::string c) {return ("461 " + c + " :Not enough parameters");}
std::string ERR_UNKNOWNCOMMAND (std::string c) {return ("421 " + c + " :Unknown command");}
std::string ERR_ERRONEUSNICKNAME (std::string c) {return ("432 " + c + " :Erroneus nickname");}

std::string ERR_BADCHANMASK (std::string c) {return ("476 " + c + " :Bad channel mask");}

