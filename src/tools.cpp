
#include "includes.hpp"

#define BUFF 1000
//get_command
// basically gnl

std::string get_command(Client &client, int &stat)
{
	std::vector<char> buffer(BUFF);
	while (client.getMsgBuffer().find('\n') == client.getMsgBuffer().npos)
	{
		std::fill(buffer.begin(), buffer.end(), 0);
		stat = recv(client.getFd(), buffer.data(), BUFF, MSG_DONTWAIT);
		if (stat == -1)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("  recv() failed");
			return "";
		}
		if (stat == 0)
			return "";
		client.addMsgBuffer(buffer.data());
	}

//keep leftovers in client->msgbuffer
	std::string msg = client.getMsgBuffer().substr(0, client.getMsgBuffer().find_first_of('\n'));
	client.setMsgBuffer(client.getMsgBuffer().substr( client.getMsgBuffer().find_first_of('\n') + 1 ));

//return till /n
	return (msg);
}
/*
	check allowed message chars
*/
bool allowed_symbol_check(std::string params) {
	for (size_t i = 0; i < params.length(); ++i) {
		if (isalnum(params[i]) == false && params[i] != '\r') //maybe should be expanded
			return false;
	}
	return true;
}


std::vector<std::string> split(std::string str, std::string separator)
{
	std::vector<std::string> splot;
	std::string token;

	while (!str.empty())
	{
		if (str.find(separator) != str.npos)
		{
			token = str.substr(0, str.find(separator));
			str = str.substr(str.find(separator) + separator.size());
		}
		else
		{
			token = str;
			str = "";
		}
		if (!token.empty())
			splot.push_back(token);
	}

	return splot;
}

/*
	Send the series of RPLs needed to greet a new client to the server
 */
void Server::welcomeClient(Client &c)
{
	c.setRegistered();
	serverReply(c, RPL_WELCOME(c));
	serverReply(c, RPL_YOURHOST(c));
	serverReply(c, RPL_CREATED(c));
	serverReply(c, RPL_MYINFO(c));
}


/*
	checks that the channel name is correct, and lowercases it.
	**** rfc1459#section-1.3
		Channels names are strings (beginning with a '&' or '#' character) of length up to 200 characters.
		Apart from the the requirement that the first character being either '&' or '#'; the only restriction
		on a channel name is that it may not contain any spaces (' '), a control G (^G or ASCII 7), or a comma
		(',' which is used as a list item separator by the protocol).
	****
 */
int getChannelName(std::string &cname)
{
	std::string ch_prefix = "&#!+"; // previously was only "&#"

	if (cname.length() > 50)
		return -1;

	if (cname.find(",") != cname.npos
		|| cname.find(" ") != cname.npos
		|| cname.find(7) != cname.npos)
		return -1;

	if (ch_prefix.find(cname[0]) == ch_prefix.npos)
		return -1;

	for (size_t i = 0; i <  cname.length(); i++)
		std::tolower(cname[i]);

	return (0);
}
