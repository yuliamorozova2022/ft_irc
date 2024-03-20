#include <iostream>
#include <string>

#include "Server.hpp"

#define BUFF 1000
//get_command
// basically gnl
//

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

bool allowed_symbol_check(std::string params) {
    for (int i = 0; i < params.length(); ++i) {
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
			str = str.substr(str.find(separator) + 1);
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
