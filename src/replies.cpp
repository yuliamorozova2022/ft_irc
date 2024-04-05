#include "includes.hpp"

// ################################################
#define USER_NOT_REGISTERED "User not registered :("
#define ERR_NOTREGISTERED "451 :You have not registered"
#define ERR_ALREADYREGISTRED "462 :You may not reregister"
#define ERR_PASSWDMISMATCH "464 :Password incorrect"

// ################################################

//note: RPL_NAMES is too complicated to structure, is in NAMES command function.

//RPLS
std::string RPL_WELCOME (Client &c) {return ("001 " + c.getNickName() + " :Welcome to our amazzzzing server! " + c.getNickName());}
std::string RPL_YOURHOST (Client &c) {return ("002 " + c.getNickName() + " :your host is IRC42");}
std::string RPL_CREATED (Client &c) {return ("003 " + c.getNickName() + " :This server is not yet created");}
std::string RPL_MYINFO (Client &c) {return ("004 " + c.getNickName() + " :IRC42");}
std::string RPL_ENDOFNAMES (Client &c, Channel &ch) {
	return ("366 " + c.getNickName() + " " + ch.getName() + " :End of NAMES list");
	}
std::string RPL_INVITING (Client &c1, Client &c2, Channel &ch) {return ("341 " + c1.getNickName() + " " + c2.getNickName() + " "  + ch.getName());}
std::string RPL_TOPIC (Client &c, Channel &ch) {return ("332 " + c.getNickName() +" "+ ch.getName() + " :" + ch.getTopic());}
std::string RPL_NOTOPIC (std::string c) {return ("331 " + c + " :No topic is set");}
std::string RPL_LIST (Client &c, Channel &ch) {
	std::stringstream ss;
	ss << ch.getOnline();
	return ("322 " + c.getNickName() + " " + ch.getName() + " :" + ss.str().c_str());
	}
std::string RPL_LISTEND (Client &c) {return ("323 " + c.getNickName() + " :End of LIST");}



//ERRS
std::string ERR_NORECIPIENT (std::string c) {return ("411 :No recipient given (" + c + ")");}
std::string ERR_NOSUCHNICK (std::string c) {return ("401 " + c + " :No such nick/channel");}
std::string ERR_NOSUCHCHANNEL (std::string c) {return ("403 " + c + " :No such channel");}
std::string ERR_NICKNAMEINUSE (std::string c) {return ("433 " + c + " :Nickname is already in use");}
std::string ERR_NEEDMOREPARAMS (std::string c) {return ("461 " + c + " :Not enough parameters");}
std::string ERR_UNKNOWNCOMMAND (std::string c) {return ("421 " + c + " :Unknown command");}
std::string ERR_ERRONEUSNICKNAME (std::string c) {return ("432 " + c + " :Erroneus nickname");}
std::string ERR_USERONCHANNEL (Client &c, Channel &ch) {return ("443 " + ch.getName() +" "+ c.getNickName() + " :is already on channel");}
std::string ERR_NOTONCHANNEL (std::string c) {return ("442 " + c + " :You're not on that channel");}
std::string ERR_INVITEONLYCHAN (std::string c) {return ("473 " + c + " ::Cannot join channel (+i)");}
std::string ERR_BADCHANMASK (std::string c) {return ("476 " + c + " :Bad channel mask");}
std::string ERR_CHANOPRIVSNEEDED (std::string c) {return ("482 " + c + " :You're not channel operator");}
std::string ERR_UNKNOWNMODE (std::string mode, std::string channel) {return ("472 " + mode + " :is unknown mode char to me for " + channel);}
std::string ERR_USERNOTINCHANNEL (std::string user, std::string channel) {return ("441 " + user + " " + channel +" :They aren't on that channel");}
