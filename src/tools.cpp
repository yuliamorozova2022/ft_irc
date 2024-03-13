#include <iostream>
#include <string>

#include "Server.hpp"

#define BUFF 1000
//get_command
// basically gnl
//

std::string get_command(Client &client)
{
	std::vector<char> buffer(BUFF);
	std::string msg = client.getMsgBuffer();
	int bytes;

	while (msg.find('\n') == msg.npos)
	{
		std::fill(buffer.begin(), buffer.end(), 0);
		bytes = recv(client.getFd(), buffer.data(), BUFF, MSG_DONTWAIT);
		if (bytes == -1)
			throw std::runtime_error("  recv() failed");
		if (bytes == 0)
			throw(std::out_of_range("WTF IS HAPPENING"));
		msg.append(buffer.data());
	}

//keep leftovers in lient->msgbuffer
	client.setMsgBuffer(msg.substr(msg.find_first_of('\n') + 1));

//return till /n
	return (msg.substr(0, msg.find_first_of('\n')));
}